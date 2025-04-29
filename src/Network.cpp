#include "Network.h"

void init_paremeters(int argc, char**argv, model& our_model)
{
	/*our_model.network_fileName = (char*)"../../../Datasets/Testing/Testing_network";
	our_model.out_fileName = (char*)"./Results/Testing_M6_L5_D5_s5_i1";
	our_model.method = 6;
	our_model.L = 5;
	our_model.D = 5;
	our_model.start_tau = 5;./main 
	our_model.incr_tau = 1;*/

	our_model.network_fileName = argv[1];
	our_model.out_fileName = argv[2];
	our_model.method = atoi(argv[3]);
	our_model.L = atoi(argv[4]);
	our_model.D = atoi(argv[5]);
	our_model.start_tau = atof(argv[6]);
	our_model.incr_tau = atof(argv[7]);

	our_model.is_sam=false;
	if(argc==9){
		our_model.epsilon = atof(argv[8]);
		our_model.is_sam=true;
	}



	our_model.tau_vec = new double[our_model.D];
	our_model.multiple_K_values = new double*[our_model.L];
	for (int l = 0; l < our_model.L; l++)
		our_model.multiple_K_values[l] = new double[our_model.D];

	for (int l = 0; l < our_model.L; l++)
		for (int t = 0; t < our_model.D; t++)
			our_model.multiple_K_values[l][t] = 0;

	for (int t = 0; t < our_model.D; t++)
		our_model.tau_vec[t] = our_model.start_tau + our_model.incr_tau*t;
}

void load_Network(model& our_model)
{
	fstream network_file;
	int num_nodes;
	int num_edges;
	int num_points;

	network_file.open(our_model.network_fileName);
	if (network_file.is_open() == false)
	{
		cout << "Cannot open this network file." << endl;
		exit(0);
	}

	network_file >> num_nodes;
	network_file >> num_edges;
	our_model.num_edges = num_edges;
	our_model.num_nodes = num_nodes;
	our_model.num_points = 0;

	//Process edge_set
	our_model.edge_set = new Edge[num_edges];
	init_PS_memory_edges(our_model, num_edges);

	if(our_model.method==7)
		init_sampling_memory(our_model);

	for (int e = 0; e < num_edges; e++)
	{
		network_file >> our_model.edge_set[e].n1;
		network_file >> our_model.edge_set[e].n2;
		network_file >> our_model.edge_set[e].length;
		network_file >> num_points;
		
		our_model.num_points += num_points;
		for (int p = 0; p < num_points; p++)
		{
			Point pt;
			pt.edge_index = e;
			network_file >> pt.dist_n1;
			pt.dist_n2=our_model.edge_set[e].length-pt.dist_n1;
			// network_file >> pt.dist_n2;
			our_model.edge_set[e].PS_vec[0].push_back(pt);
		}
	}

	generate_points(our_model);


	//Process the road network
	vector<int> tampVec;
	for(int v=0;v<our_model.num_nodes;v++){
		our_model.Network.push_back(tampVec);
	}
	for(int e=0;e<our_model.num_edges;e++){
		our_model.Network[our_model.edge_set[e].n1].push_back(e);
		our_model.Network[our_model.edge_set[e].n2].push_back(e);
	}

	network_file.close();
}

void output_multiple_results(model& our_model)
{
	fstream output_file;

	output_file.open(our_model.out_fileName, ios::in | ios::out | ios::trunc);
	if (output_file.is_open() == false)
	{
		cout << "Cannot open the output file!" << endl;
		exit(0);
	}
	output_file << setprecision(10)<<fixed;
	//The K-function values for the original dataset
	output_file << "The K-function values for the original dataset: " << endl;
	for (int d = 0; d < our_model.D; d++)
		output_file << our_model.multiple_K_values[0][d] << " ";
	output_file << endl;

	output_file << "The K-function values for the " << our_model.L - 1 << " random datasets: " << endl;
	for (int l = 1; l < our_model.L; l++)
	{
		for (int d = 0; d < our_model.D; d++)
			output_file << our_model.multiple_K_values[l][d] << " ";
		output_file << endl;
	}

	// Record experimental information
	output_file <<endl<< "Method" <<our_model.method<< " runtime:  " <<our_model.runtime<< "s" <<endl;

	if (our_model.is_sam)
		output_file << "epsilon:"<< our_model.epsilon <<" ";
	output_file << "L:" <<our_model.L << "  D:"<<our_model.D << "  start_tau:"<< our_model.start_tau <<"  incr_tau:"<< our_model.incr_tau;

}

void init_PS_memory_edges(model& our_model, int num_edges)
{
	vector<Point> temp_vec{};

	for (int e = 0; e < num_edges; e++)
		for (int s = 0; s < our_model.L; s++){
			our_model.edge_set[e].PS_vec.push_back(temp_vec);
			// our_model.edge_set[e].sam_PS_vec.push_back(temp_vec);
		}
}
void init_sampling_memory(model& our_model)
{
	int sam_size=ceil(1.0/our_model.epsilon);
	int size=our_model.D*sam_size;

	our_model.minheap.resize(2);

	our_model.sam_edge=vector<Point>{};
}

void generate_points(model& our_model)
{
	default_random_engine generator_int;
	default_random_engine generator_double;
	int edge_index;
	Point pt;

	uniform_int_distribution<int> distribution(0, our_model.num_edges - 1);

	for (int l = 1; l < our_model.L; l++)
	{
		for (int i = 0; i < our_model.num_points; i++)
		{
			edge_index = distribution(generator_int);
			vector<Point>& PS_cur = our_model.edge_set[edge_index].PS_vec[l];
			uniform_real_distribution<double> distribution_double(0.0, our_model.edge_set[edge_index].length);
			pt.dist_n1 = distribution_double(generator_double);
			pt.dist_n2 = our_model.edge_set[edge_index].length - pt.dist_n1;
			pt.edge_index = edge_index;

			PS_cur.push_back(pt);
		}
	}
}

void sort_data_points(model& our_model)
{
	//Sorting the data points
	if (our_model.method >= 3 && our_model.method <= 8)
	{
		for (int e = 0; e < our_model.num_edges; e++)
			for(int l=0; l < our_model.L; l++){
				// if (our_model.edge_set[e].PS_vec[l].size()==0)
				// 	continue;
				sort(our_model.edge_set[e].PS_vec[l].begin(), our_model.edge_set[e].PS_vec[l].end());
			}
	}
}


void sort_selection(model& our_model, int e_index) {
	vector<Point>& PS=our_model.edge_set[e_index].PS_vec[our_model.cur_data_index];
	our_model.sam_edge=vector<Point>{};
	int sam_size=ceil(1.0/our_model.epsilon);
	if(sam_size>=PS.size()){
		our_model.sam_edge=PS;
		return;
	}
	for(int i=1;i<sam_size+1;i++){
		int sam_idx=ceil((i-1.0/2)*1.0/sam_size*PS.size()) - 1;
		our_model.sam_edge.push_back(PS[sam_idx]);
	}
}
