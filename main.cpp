#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <fstream>
#include <sstream>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

bool sort_function(const std::string &lhs, const std::string &rhs)
{
    std::vector<std::string> lhsList = split(lhs, '.');
    std::vector<std::string> rhsList = split(rhs, '.');

    bool result;
    if(lhsList.at(0) == rhsList.at(0)){
        if(lhsList.at(1) == rhsList.at(1)){
            if(lhsList.at(2) == rhsList.at(2)){
                result = std::stoi(lhsList.at(3), nullptr, 0) > std::stoi(rhsList.at(3), nullptr, 0);
            }else{
                result = std::stoi(lhsList.at(2), nullptr, 0) > std::stoi(rhsList.at(2), nullptr, 0);
            }
        }else{
            result = std::stoi(lhsList.at(1), nullptr, 0) > std::stoi(rhsList.at(1), nullptr, 0);
        }
    }else{
        result = std::stoi(lhsList.at(0), nullptr, 0) > std::stoi(rhsList.at(0), nullptr, 0);
    }
    return result;
}

std::vector<std::string> filter(int value, std::vector<std::string> &vector)
{
    std::stringstream ss;
    std::string strValue;
    ss << value;
    ss >> strValue;

    std::vector<std::string> temp;
    for(std::string ip: vector){
        std::vector<std::string> listBytes = split(ip, '.');
        if(listBytes.at(0) == strValue){
            temp.emplace_back(ip);
        }
    }
    return temp;
}

std::vector<std::string> filter(int value1, int value2, std::vector<std::string> &vector)
{
    std::stringstream ss1, ss2;
    std::string strValue1, strValue2;
    ss1 << value1;
    ss1 >> strValue1;
    ss2 << value2;
    ss2 >> strValue2;

    std::vector<std::string> temp;
    for(std::string ip: vector){
        std::vector<std::string> listBytes = split(ip, '.');
        if(listBytes.at(0) == strValue1 && listBytes.at(1) == strValue2){
            temp.emplace_back(ip);
        }
    }
    return temp;
}

std::vector<std::string> filter_any(int value, std::vector<std::string> &vector)
{
    std::stringstream ss;
    std::string strValue;
    ss << value;
    ss >> strValue;

    std::vector<std::string> temp;
    for(std::string ip: vector){
        std::vector<std::string> listBytes = split(ip, '.');
        if(listBytes.at(0) == strValue || listBytes.at(1) == strValue ||
           listBytes.at(2) == strValue || listBytes.at(3) == strValue){
           temp.emplace_back(ip);
        }
    }
    return temp;
}

int main(int argc, char const *argv[])
{
    std::string line;
    std::ifstream in("C://temp/ip_filter.tsv"); // окрываем файл для чтения

    if(!in.is_open()) return 1;

    std::vector<std::vector<std::string> > ip_pool;
    std::vector<std::string> ip_clean;
    while (getline(in, line))
    {
        std::vector<std::string> v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
        std::cout << ip_pool.at(0)[1] << std::endl;

        ip_clean.push_back(v.at(0));
    }
    in.close();     // закрываем файл

    // TODO reverse lexicographically sort
    std::sort(ip_clean.begin(), ip_clean.end(), sort_function);

    // TODO filter by first byte and output - ip = filter(1)
    std::vector<std::string> f1Vector = filter(1, ip_clean);

    // TODO filter by first and second bytes and output - ip = filter(46, 70)
    std::vector<std::string> f2Vector = filter(46, 70, ip_clean);

    // TODO filter by any byte and output - ip = filter_any(46)
    std::vector<std::string> f3Vector = filter_any(46, ip_clean);

    std::ofstream out;            // поток для записи
    out.open("C://temp/out.txt"); // окрываем файл для записи
    if (out.is_open())
    {
        for(std::string ip: ip_clean)
            out << ip << std::endl;
        out << std::endl;
        for(std::string ip: f1Vector)
            out << ip << std::endl;
        out << std::endl;
        for(std::string ip: f2Vector)
            out << ip << std::endl;
        out << std::endl;
        for(std::string ip: f3Vector)
            out << ip << std::endl;
    }
    out.close();



    try
    {
        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort

        for(std::vector<std::vector<std::string> >::const_iterator ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
        {
            for(std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if (ip_part != ip->cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
