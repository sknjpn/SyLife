#include "Field.h"
#include "nanoflann.hpp"
#include "Rigidbody.h"

unique_ptr<Field>	g_field;

Field::Field()
{
	/*
	RigidbodyCloud cloud;

	my_kd_tree_t   index(3 , cloud, KDTreeSingleIndexAdaptorParams(10));
	index.buildIndex();

	const num_t query_pt[3] = { 0.5, 0.5, 0.5 };

	// ----------------------------------------------------------------
	// knnSearch():  Perform a search for the N closest points
	// ----------------------------------------------------------------
	{
		size_t num_results = 5;
		std::vector<size_t>   ret_index(num_results);
		std::vector<num_t> out_dist_sqr(num_results);

		num_results = index.knnSearch(&query_pt[0], num_results, &ret_index[0], &out_dist_sqr[0]);

		// In case of less points in the tree than requested:
		ret_index.resize(num_results);
		out_dist_sqr.resize(num_results);

		cout << "knnSearch(): num_results=" << num_results << "\n";
		for (size_t i = 0; i < num_results; i++)
			cout << "idx[" << i << "]=" << ret_index[i] << " dist[" << i << "]=" << out_dist_sqr[i] << endl;
		cout << "\n";
	}

	// ----------------------------------------------------------------
	// radiusSearch(): Perform a search for the points within search_radius
	// ----------------------------------------------------------------
	{
		const num_t search_radius = static_cast<num_t>(0.1);
		std::vector<std::pair<size_t, num_t> >   ret_matches;

		nanoflann::SearchParams params;
		//params.sorted = false;

		const size_t nMatches = index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

		cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
		for (size_t i = 0; i < nMatches; i++)
			cout << "idx[" << i << "]=" << ret_matches[i].first << " dist[" << i << "]=" << ret_matches[i].second << endl;
		cout << "\n";
	}
	*/
}


Field::~Field()
{
}

void Field::Start()
{
}
