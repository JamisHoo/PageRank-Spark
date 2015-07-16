/******************************************************************************
 *  Copyright (c) 2015 Jamis Hoo
 *  Distributed under the MIT license 
 *  (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *  
 *  Project: 
 *  Filename: query.cc 
 *  Version: 1.0
 *  Author: Jamis Hoo
 *  E-mail: hjm211324@gmail.com
 *  Date: Jul 16, 2015
 *  Time: 09:07:36
 *  Description: 
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

int main(int argc, char** argv) {
    using namespace std;

    size_t top_k = 10;

    if (argc > 1) top_k = stoull(argv[1]);

    ifstream fin("pagerank_result");

    vector< tuple<float, size_t, string> > top_k_items;
    
    size_t max_word_index = 0;

    string line;
    for (size_t i = 0; i < top_k; ++i) {
        getline(fin, line);

        line = line.substr(1, line.length() - 2);

        float pr = stof(line.substr(0, line.find_first_of(',')));
        size_t word_index = stoull(line.substr(line.find_first_of(',') + 1));

        max_word_index = max(word_index, max_word_index);

        top_k_items.emplace_back(pr, word_index, "");
    }

    fin.close();

    fin.open("dictionary");

    for (size_t i = 0; i < max_word_index + 1; ++i) {
        getline(fin, line);

        for (auto& j: top_k_items) 
            if (get<1>(j) == i) 
                get<2>(j) = line;
    }

    for (const auto& i: top_k_items)
        cout << get<0>(i) << ' ' << get<2>(i) << '\n';
}


