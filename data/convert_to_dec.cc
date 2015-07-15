/******************************************************************************
 *  Copyright (c) 2015 Jamis Hoo
 *  Distributed under the MIT license 
 *  (See accompanying file LICENSE or copy at http://opensource.org/licenses/MIT)
 *  
 *  Project: PageRank
 *  Filename: convert_to_dec.cc 
 *  Version: 1.0
 *  Author: Jamis Hoo
 *  E-mail: hoojamis@gmail.com
 *  Date: Jul 15, 2015
 *  Time: 22:09:30
 *  Description: 
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, uint32_t> dict;
std::vector<std::string> inverted_dict;

std::ofstream dec_output;


inline uint32_t find_no(const std::string& url) {
    auto ite = dict.find(url);
    if (ite == dict.end()) {
        uint32_t no = dict.size();
        dict.emplace(url, no);
        inverted_dict.emplace_back(url);
        return no;
    }
    return ite->second;
}

inline void input(const char* line, const size_t line_length) {
    const char* header = "<page>    <title>";
    const char* tailer = "</title>";
    const size_t header_length = 17;
    const size_t tailer_length = 8;

    if (line_length < header_length + tailer_length) return;

    size_t i;

    for (i = 0; i < header_length; ++i)
        if (line[i] != header[i]) return;


    size_t match_length = 0;
    for (i = header_length; i < line_length; ++i)
        if (line[i] == tailer[match_length]) {
            ++match_length;
            if (match_length == tailer_length) break;
        } else {
            match_length = 0;
        }

    if (match_length != tailer_length) return;

    ++i;
    
    std::string URL(line + header_length, i - header_length - tailer_length);

    uint32_t URL_no = find_no(URL);

    dec_output << URL_no << ' ';

    while (i < line_length) {
        for (; i < line_length - 1; ++i) 
            if (line[i] == '[' && line[i + 1] == '[') break;
        size_t j;
        for (j = i + 2; j < line_length - 1; ++j)
            if (line[j] == ']' && line[j + 1] == ']') {
                // [i, j] is a url
                uint32_t url_no = find_no(std::string(line + i + 2, j - i - 2));
                dec_output << url_no << ' ';
                i = j + 2;
                break;
            }

        if (j >= line_length - 1) break;
    }
    dec_output << '\n';
}


int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(false);

    if (argc < 2) return 0;

    std::ifstream fin(argv[1]);

    dec_output.open("dec_input");

    size_t counter = 0;

    std::string line;
    while (getline(fin, line)) {
        counter++;
        if (counter % 8192 == 0)
            std::cout << counter << std::endl;
        input(line.data(), line.length());
    }

    // output dict
    std::ofstream fout("dictionary");
    for (const auto& i: inverted_dict) 
        fout << i << '\n';
}
