#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>
#include <string>
#include <string.h>
#include <ctime>
#include <future>
#include <atomic>
#include <vector>
#include <ratio>
#include <chrono>
#include <openssl/evp.h>

namespace Dictionary
{
    static const char syms[] = "abcdefghijklmnopqrstuvwxyz\0";
}

namespace BruteHash
{
    typedef std::pair<std::string, std::string> HashPair;
    typedef std::list<HashPair> HashRes;

    bool compute_hash(const std::string &unhashed, std::string &hashed)
    {
        bool success{false};
        EVP_MD_CTX *context = EVP_MD_CTX_new();
        if (context != NULL)
        {
            if (EVP_DigestInit_ex(context, EVP_sha256(), NULL))
                if (EVP_DigestUpdate(context, unhashed.c_str(), unhashed.length()))
                {
                    unsigned char hash[EVP_MAX_MD_SIZE];
                    unsigned int lengthOfHash{0};

                    if (EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                    {
                        std::stringstream ss;
                        for (unsigned int i = 0; i < lengthOfHash; ++i)
                            ss << std::hex << std::setw(2)
                               << std::setfill('0') << (int)hash[i];

                        hashed = ss.str();
                        success = true;
                    }
                }
            EVP_MD_CTX_free(context);
        }
        return success;
    }

    bool next_password(std::string &password)
    {
        size_t len = password.length();
        for (size_t i = len - 1; i >= 1; --i)
        {
            char c = password[i];
            if (c < 'z')
            {
                ++password[i];
                return true;
            }
            password[i] = 'a';
        }
        return false;
    }

    void multi_thread(const std::list<std::string>& hashes, char s1)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        std::string pass = "aaaaa";
        pass[0] = s1;
        auto count = hashes.size();
        while (count > 0)
        {
            std::string hashed;
            if (compute_hash(pass, hashed))
            {
                for (auto hash : hashes)
                {
                    if (hash == hashed)
                    {
                        auto t2 = std::chrono::high_resolution_clock::now();
                        --count;
                        std::cout << "Password is '" << pass 
                            << "' Hash is '" << hash << std::endl;
                        auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
                        std::cout << "Total time[sec]: " << duration.count() << std::endl;
                        break;
                    }
                }
            }
            if (!next_password(pass))
                break;
        }
    }

    HashPair single_thread(const std::string& hash, time_t& total)
    {
        auto start_time = clock();
        for (auto s1 : Dictionary::syms)
            for (auto s2 : Dictionary::syms)
                for (auto s3 : Dictionary::syms)
                    for (auto s4 : Dictionary::syms)
                        for (auto s5 : Dictionary::syms)
                        {
                            std::string pass{{s1, s2, s3, s4, s5}};
                            std::string hashed;
                            if (compute_hash(pass, hashed) && hash == hashed)
                            {
                                total = clock() - start_time;
                                return HashPair{pass, hash};
                            }
                        }

        return HashPair{};
    }
}

int main(int argc, char** argv)
{
    const char* hash_file = "hashes.txt";
    std::ifstream input;
    input.open(hash_file);
    if (input.fail())
        std::cerr << "Failed to open " << hash_file << std::endl;
    std::list<std::string> hash_list;
    std::string hash;
    while (std::getline(input, hash))
        hash_list.push_back(hash);
    input.close();
    if (argc <= 1)
    {
        std::cout << "Run in single-thread\n\n";
        for (auto hash : hash_list)
        {
            time_t cur_time;
            auto result = BruteHash::single_thread(hash, cur_time);
            if (!result.first.empty())
                std::cout << "Password is '" << result.first
                          << "' Hash is '" << result.second
                          << "'\n Total time[msec]: " << cur_time / 1000.0 << std::endl;
        }
        std::cout << std::endl;
        return 0;
    }
    else
    {
        if (strcmp(argv[1], "-m") != 0)
            return 1;
        std::cout << "Run in multi-thread\n\n";
        const int n = 26;
        std::vector<std::future<void>> futures;
        for (int i = 0; i < n; ++i)
        {
            char c = 'a' + i;
            futures.push_back(
                std::async(std::launch::async, [hash_list, c]()
                           { BruteHash::multi_thread(hash_list, c); }));
        }
        return 0;
    }

    return 0;
}