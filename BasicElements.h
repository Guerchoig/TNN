#pragma once
#include "Common.h"

// Nof neurons and layers
constexpr layer_dim view_cols = 10;
constexpr layer_dim view_rows = 10;
constexpr layer_dim neurons_in_layer = view_cols * view_rows;
constexpr layer_dim encoder_layers = 2;
constexpr layer_dim decoder_layers = 2;
constexpr layer_dim total_layers = encoder_layers + decoder_layers;

constexpr layer_dim total_neurons = neurons_in_layer * total_layers;

// Nof connections per neuron = exp(-alpha * (distance - 1)) * (sqr(distance) * 3 + distance * 6 + 4 ) * 2
constexpr float alpha{ 0.79f }; 
constexpr layer_dim max_distance{ 6 }; // 1 < distance <= max_distance

// Events buffer size
constexpr int needed_ticks_in_second = 10000;
constexpr unsigned int events_buffer_length = 10 * needed_ticks_in_second;

constexpr Potential starting_threshold { 1.0f };
constexpr Potential starting_rest_potential { 0.1f };

// Masks for flags
constexpr unsigned short OCCUPIED = 0x00FF;
constexpr unsigned short HAS_EXTENTION = 0xFF00;

// Accelerator for ribs calculations
constexpr layer_dim acc[6][2]{
	{0, 0 / 2}, {0, 1 / 2}, {0, 2 / 2}, {0, 3 / 2}, {0, 4 / 2}, {0, 5 / 2}
};

using ViewField = unsigned char[view_rows][view_cols];

using namespace std;

enum synapse_t : unsigned char {
	Glutamate = 0,
	GABA,
	Acetylcholine,
	Dopamine,
	Serotonin,
	Norepinephrine,
	Epinephrine
};

enum neur_address : layer_dim {
	row = 0,
	col,
	layer
};

union NeuronAddress {
	layer_dim ar[3]; //row
	struct {
		layer_dim r;
		layer_dim c;
		layer_dim l;
	} st{ 0,0,0 };
}; 


struct EventBufferMember {
	unsigned short	flags{ 0 };
	NeuronAddress src_main_neuron_address;
	vector<NeuronAddress>* p_other_addresses = 0;
};


class EventBuffer {
private:
	EventBufferMember buffer[events_buffer_length];

public:
	
	void put(const NeuronAddress& src_neuron, const clock_count_t t_fired) {
		
		buffer_time_index index = t_fired % events_buffer_length;
		EventBufferMember& eb_item = buffer[index];

		if (eb_item.flags & OCCUPIED) {
			if (!(eb_item.flags & HAS_EXTENTION)) {
				eb_item.p_other_addresses = new vector<NeuronAddress>;
				eb_item.flags |= HAS_EXTENTION;
			}
			eb_item.p_other_addresses->push_back(src_neuron);
		}
		else
			eb_item.src_main_neuron_address = src_neuron;
	};
	
	
	 //returns address of buff-item
	 //after having used the item one must call clear_item 
	 //to free the vector and zero other requisites
	EventBufferMember& get(const clock_count_t t_fired) {
		buffer_time_index index = t_fired % events_buffer_length;
		EventBufferMember& eb_item = buffer[index];
		return eb_item;
	};
	
	
	void drop_item(EventBufferMember& eb_item) {
		if (eb_item.flags & HAS_EXTENTION)
			delete eb_item.p_other_addresses;
		memset(&eb_item, 0, sizeof(EventBufferMember));
	};
};


struct ConnItem {
	layer_dim		layer = 0;
	layer_dim		row = 0;
	layer_dim		col = 0;
	clock_count_t	last_fired;
	weight_t		weight;
	proximity		length;
	synapse_t		type;
};

using Connections = vector<ConnItem>;

struct Neuron {
public:
	Potential		potential = 0;
	Potential		threshold = 0;
	Connections*	connections = 0;
};

inline clock_count_t net_clock_count() {
	using namespace std::chrono;
	// Get current time with precision of nanoseconds
	auto now = high_resolution_clock::now();
	// Convert time_point to integral type
	clock_count_t res = now.time_since_epoch().count();
	return res;
};

class VisualDetector {
private:

	ViewField curr_scene = { 0 };
	ViewField prev_scene = { 0 };
	std::thread detection_th;

public:
	VisualDetector(const EventBuffer& buffer) {
		//std::thread t_th(&proceed_changes, this, buffer);
		//detection_th = std::move(t_th);
	};
	~VisualDetector(void) {
		detection_th.join();
	};
	void proceed_changes(EventBuffer& buffer) {
		for (layer_dim row = 0; row < view_rows; +row)
			for (layer_dim col = 0; col < view_rows; +col)
				if (curr_scene[row][col] != prev_scene[row][col]) {
					NeuronAddress addr{ 0, row, col };
					using namespace std::chrono;
					auto now = high_resolution_clock::now();
					const clock_count_t event_clock = now.time_since_epoch().count();
					buffer.put(addr, event_clock);
				}
	};

};


class Brain {
private:
	struct Pair{
		layer_dim row; 
		layer_dim col; 
	};
	Neuron neurons[total_layers][view_rows][view_cols];
	EventBuffer ebuff;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;

	
	friend void print_neurons(Brain* brain)
	{
		Neuron (& _neurons)[total_layers][view_rows][view_cols] = brain->neurons;
		for (layer_dim l = 0; l < encoder_layers + decoder_layers; ++l) {
			for (layer_dim row = 0; row < view_rows; ++row) {
				for (layer_dim col = 0; col < view_rows; ++col) {
					Neuron& neu = _neurons[l][row][col];
					cout << neu.potential << ' ' << neu.threshold << ' ' << neu.connections << "   ";
				};
				cout << '\n';
			};
			cout << '\n';
		};
	};

	friend void print_connections(Brain* brain)
	{
		Neuron(&_neurons)[total_layers][view_rows][view_cols] = brain->neurons;
		for (layer_dim l = 0; l < encoder_layers + decoder_layers; ++l) {
			for (layer_dim row = 0; row < view_rows; ++row) {
				for (layer_dim col = 0; col < view_rows; ++col) {
					for (layer_dim cc = 0; cc < _neurons[l][row][col].connections->size(); ++cc) {
						ConnItem * conn = &(*_neurons[l][row][col].connections)[cc];
						cout << conn->layer << ' ' << conn->row << ' ' << conn->col << ' '<< conn->last_fired << ' ' <<conn->length << ' '<< conn->type << ' ' << conn->weight << "   ";
					}
				};
				cout << '\n';
			};
			cout << '\n';
		};
	};
	
	int nof_neurons_at_distance(layer_dim distance) {
		layer_dim two_d_minus = distance * 2 - 1;
		layer_dim res = two_d_minus * two_d_minus * 6 + two_d_minus * 12 + 8;
		return(res);
	}

	vector<NeuronAddress> *distribute_points(NeuronAddress& point, layer_dim distance) {
		// Creating return points vector
		layer_dim max_nof_points_at_distance = nof_neurons_at_distance(distance);
		auto res = new vector<NeuronAddress>[max_nof_points_at_distance];
		// Add 8 vertexes
		for (auto r1 = point.st.r - distance; r1 <= point.st.r + distance; r1 += 2 * distance)
			for (auto c1 = point.st.c - distance; c1 <= point.st.c + distance; c1 += 2 * distance)
				for (auto l1 = point.st.l - distance; l1 <= point.st.l + distance; l1 += 2 * distance) {
					NeuronAddress addr{ r1, c1, l1 };
					res->push_back(addr);
				}
		// Add 12 ribs, each rib is formed by two perpendicular edges; nof edges is 6
		layer_dim ribs[12][3];
		for( auto i = 2; i < 6; ++ i)
			for(auto j = 0; acc[j][1] < acc[i][1]; ++j)

		return res;
	};

public:
	Brain(void) {

		default_random_engine gen;
		uniform_real_distribution<float> distribution(0.0, 1.0);

		// Init neurons
		for (layer_dim layer = 0; layer < total_layers; ++layer) {
			for (layer_dim row = 0; row < view_rows; ++row) {
				for (layer_dim col = 0; col < view_cols; ++col) {

					// Init neuron requisites
					Neuron& neuron = neurons[layer][row][col];
					neuron.potential = distribution(gen);
					neuron.threshold = starting_threshold;

					// Init neuron connections
					neuron.connections = new Connections;
					
					for (layer_dim distance = 1; distance <= max_distance; ++distance) {
						for()
					};
					
				};
			};
		};
		start = std::chrono::high_resolution_clock::now();

	};
	void process_net_events(void) {
		
	}
};