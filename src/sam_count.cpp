#include"sam_count.h"

void sam_edge_edge_count2(model& our_model, int wide_e_index, int e_index){

	vector<Point>& PS_wide_e = our_model.edge_set[wide_e_index].PS_vec[our_model.cur_data_index];
	vector<Point>& PS = our_model.edge_set[e_index].PS_vec[our_model.cur_data_index];
    vector<Point>& sam_PS_wide = our_model.is_sam ? our_model.sam_edge : our_model.edge_set[wide_e_index].PS_vec[our_model.cur_data_index];

    double sam_factor=PS_wide_e.size()/sam_PS_wide.size();

	int pos_au, pos_av, pos_bu, pos_bv;
	int node_u, node_v;
	double cur_tau_au, cur_tau_av, cur_tau_bu, cur_tau_bv;

	if (PS_wide_e.empty() || PS.empty())
		return;

	node_u = our_model.edge_set[e_index].n1;
	node_v = our_model.edge_set[e_index].n2;


    for(int d=our_model.D-1;d>=0;d--){

    if (our_model.sp_node_vec_first[node_u].cur_sp_value > our_model.tau_vec[d] &&
        our_model.sp_node_vec_first[node_v].cur_sp_value > our_model.tau_vec[d] &&
        our_model.sp_node_vec_second[node_u].cur_sp_value > our_model.tau_vec[d] &&
        our_model.sp_node_vec_second[node_v].cur_sp_value > our_model.tau_vec[d])
        break;


	//initialization of positions
	//****************************************************************************//
	bool au_end = false;
	bool bu_end = false;
	cur_tau_au = our_model.tau_vec[d] - our_model.sp_node_vec_first[node_u].cur_sp_value;
	cur_tau_bu = our_model.tau_vec[d] - our_model.sp_node_vec_second[node_u].cur_sp_value - our_model.edge_set[wide_e_index].length;
	pos_au = -1; pos_bu = -1;
	for (int ii = 0; ii < (int)PS.size(); ii++)
	{
		if (au_end == true && bu_end == true)
			break;

		if (au_end == false)
		{
			if (PS[ii].dist_n1 < cur_tau_au)
				pos_au++;
			else
				au_end = true;
		}

		if (bu_end == false)
		{
			if (PS[ii].dist_n1 < cur_tau_bu)
				pos_bu++;
			else
				bu_end = true;
		}
	}

	bool av_end = false;
	bool bv_end = false;
	cur_tau_av = our_model.tau_vec[d] - our_model.sp_node_vec_first[node_v].cur_sp_value;
	cur_tau_bv = our_model.tau_vec[d] - our_model.sp_node_vec_second[node_v].cur_sp_value - our_model.edge_set[wide_e_index].length;
	pos_av = -1; pos_bv = -1;
	for (int ii = PS.size() - 1; ii >= 0; ii--)
	{
		if (av_end == true && bv_end == true)
			break;

		if (av_end == false)
		{
			if (PS[ii].dist_n2 < cur_tau_av)
				pos_av++;
			else
				av_end = true;
		}

		if (bv_end == false)
		{
			if (PS[ii].dist_n2 < cur_tau_bv)
				pos_bv++;
			else
				bv_end = true;
		}
	}
	//****************************************************************************//

	//update
	double delta;
	double K_value = 0;
	for (int i = 0; i < (int)sam_PS_wide.size(); i++)
	{
		if (i == 0)
			delta = sam_PS_wide[i].dist_n1;
		else
			delta = sam_PS_wide[i].dist_n1 - sam_PS_wide[i - 1].dist_n1;

		cur_tau_au -= delta; cur_tau_av -= delta;
		cur_tau_bu += delta; cur_tau_bv += delta;

		for (int ii = pos_au; ii >= 0; ii--)
		{
			if (PS[ii].dist_n1 <= cur_tau_au)
				break;
			else
				pos_au--;
		}

		for (int ii = pos_av; ii >= 0; ii--)
		{
			if (PS[PS.size() - 1 - ii].dist_n2 <= cur_tau_av)
				break;
			else
				pos_av--;
		}

		for (int ii = pos_bu; ii < (int)PS.size() - 1; ii++)
		{
			if (PS[ii + 1].dist_n1 > cur_tau_bu)
				break;
			else
				pos_bu++;
		}

		for (int ii = pos_bv; ii < (int)PS.size() - 1; ii++)
		{
			if (PS[PS.size() - 2 - ii].dist_n2 > cur_tau_bv)
				break;
			else
				pos_bv++;
		}

		K_value += min((max(pos_au, pos_bu) + 1) + (max(pos_av, pos_bv) + 1), (int)PS.size());
	}
    // K_value*=sam_factor;

    our_model.multiple_K_values[our_model.cur_data_index][d]+= K_value*sam_factor;
    }
}


/**
 *  (e，pi)-count  
 *          = 0       [0, start_d_idx] 
 *          = cu + cv [start_d_idx, end_d_idx]
 *          = ps_wide [end_d_idx, D]  
 */
void sam_edge_edge_count3(model& our_model, int wide_e_index, int e_index){
    vector<Point>& PS_wide = our_model.edge_set[wide_e_index].PS_vec[our_model.cur_data_index];
    vector<Point>& PS = our_model.edge_set[e_index].PS_vec[our_model.cur_data_index];

	vector<Point>& sam_PS_wide = our_model.sam_edge;


    const int ps_size = PS.size();
    const int ps_wide_size = PS_wide.size();
    const int sam_PS_wide_size = sam_PS_wide.size();

    if (ps_size==0 || ps_wide_size==0) 
        return;
    
    vector<priority_queue<heapValue,vector<heapValue>,mergeCompare>>& minheap=our_model.minheap;

	int node_u = our_model.edge_set[e_index].n1;
	int node_v = our_model.edge_set[e_index].n2;

    double au = our_model.sp_node_vec_first[node_u].cur_sp_value;
    double bu = our_model.sp_node_vec_second[node_u].cur_sp_value;
    double av = our_model.sp_node_vec_first[node_v].cur_sp_value;
    double bv = our_model.sp_node_vec_second[node_v].cur_sp_value;

    int start_d_idx;
    for(start_d_idx=our_model.D;start_d_idx>0;start_d_idx--)
        if (au > our_model.tau_vec[start_d_idx-1] &&
            bu > our_model.tau_vec[start_d_idx-1] &&
            av > our_model.tau_vec[start_d_idx-1] &&
            bv > our_model.tau_vec[start_d_idx-1])
            break;

    if(start_d_idx==our_model.D)
        return;

    //calculate tau
    vector<int> end_d(sam_PS_wide_size);

    int multi_tau_num=0;
    for(int pi=0 ; pi < sam_PS_wide_size ; pi++){
        double pi_a = sam_PS_wide[pi].dist_n1;
        double pi_b = sam_PS_wide[pi].dist_n2;

        double tau_u_first = our_model.tau_vec[start_d_idx]-min(pi_a+au,pi_b+bu);
        double tau_v_first = our_model.tau_vec[start_d_idx]-min(pi_a+av,pi_b+bv);

        
        int end_d_idx = start_d_idx;
        for (;end_d_idx < our_model.D; end_d_idx++ ) //确定end_d 
        {
            double delta = our_model.tau_vec[end_d_idx] - our_model.tau_vec[start_d_idx];
            double tau_u_d = tau_u_first + delta;
            double tau_v_d = tau_v_first + delta;

            if(tau_u_d + tau_v_d >= our_model.edge_set[e_index].length)
                break;
        }
        end_d[pi] = end_d_idx;
        multi_tau_num += (end_d_idx-start_d_idx);

        if(end_d[pi]!=start_d_idx){//
            minheap[0].emplace( tau_u_first, start_d_idx, pi);
            minheap[1].emplace( tau_v_first, start_d_idx, pi);
        }
    }

    vector merged_result(2,vector<pair<double,int>>(multi_tau_num)); //pair<double,int> 存储排序后的tau_u值（or tau_v）以及所对应的距离阈值d

    //heapValue 存储多路归并的元素及其在原二维数组的位置。在该算法中存储tau值和其对应的位置信息（val:tau_u或tau_v i:距离阈值下标，j：sam_ps_wide上的第j个点）

    //merge sort 
    for(int t =0; t < multi_tau_num; t++){
        for(int node = 0 ; node < 2; node++){//handle u and v
            heapValue pop_min = minheap[node].top();
            minheap[node].pop();
            
            merged_result[node][t] = make_pair( pop_min.val,pop_min.i);

            if (pop_min.i + 1 < end_d[pop_min.j]){
                int push_i = pop_min.i + 1;
                int push_j = pop_min.j;

                double push_value = pop_min.val + our_model.incr_tau;//
                minheap[node].emplace(heapValue{push_value, push_i, push_j});
            }
        }
    }


    //d times (e,wide_e)-count
    int j = 0; int k = ps_size-1;
    int cu = 0; int cv = 0;
    for (int t =0;t<multi_tau_num;t++){
        auto & tau_u_t=merged_result[0][t];
        auto & tau_v_t=merged_result[1][t];

        for (;j<ps_size && PS[j].dist_n1 <=tau_u_t.first; j++)
            cu++;
        for(;k>=0 && PS[k].dist_n2 <= tau_v_t.first; k--)
            cv++;

        our_model.multiple_K_values[our_model.cur_data_index][tau_u_t.second] += (double)ps_wide_size/sam_PS_wide_size *cu;
        our_model.multiple_K_values[our_model.cur_data_index][tau_v_t.second] += (double)ps_wide_size/sam_PS_wide_size *cv; 
    }
    sort(end_d.begin(),end_d.end());
    for(int d=end_d[0],i=0; d < our_model.D; d++ ){
        while (i<sam_PS_wide_size && d==end_d[i])
            i++;
        our_model.multiple_K_values[our_model.cur_data_index][d] += (double)ps_wide_size/sam_PS_wide_size * ps_size *i;// [end_d_idx, D]  
    }

}

void sam_edge_edge_count_same_edge(model& our_model, int e_index){

	vector<Point>& PS = our_model.edge_set[e_index].PS_vec[our_model.cur_data_index];

    // vector<Point>& sam_PS = our_model.is_sam?our_model.edge_set[e_index].sam_PS_vec[our_model.cur_data_index] :PS;
    // int sam_ps_size= sam_PS.size();
    // double sam_factor = our_model.is_sam && PS.size()!=sam_PS.size() ? (PS.size()*(PS.size()-1))/(sam_ps_size*(sam_ps_size-1)) : 1; 


    vector<Point>& sam_PS=PS;
    double sam_factor=1; 
    int sam_ps_size= sam_PS.size();


	int left , right ;

    for(int t=0;t<our_model.D;t++){
        double cur_tau=our_model.tau_vec[t];
        double k=0;
        if (PS.back().dist_n1 - PS.front().dist_n1 <= cur_tau )
        {
            our_model.multiple_K_values[our_model.cur_data_index][t] += PS.size()*(PS.size()-1);
            continue;
        }

        left =0 ;
        right = 0;

        while (right<sam_ps_size)
        {
            while (right<sam_ps_size && sam_PS[right].dist_n1-sam_PS[left].dist_n1 <= cur_tau)
            {
                // our_model.multiple_K_values[our_model.cur_data_index][t] += 2 * (right-left);
                k +=2 * (right-left);
                // mult_k_value[t] += 2 * (right-left);
                right++;
            }
            while (right<sam_ps_size && sam_PS[right].dist_n1-sam_PS[left].dist_n1 > cur_tau)
                left++;
            
        }
         our_model.multiple_K_values[our_model.cur_data_index][t]+= sam_factor * k;
    }
    
}
