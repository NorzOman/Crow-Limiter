
#ifndef _LIMITER_H
#define _LIMITER_H

#include "crow.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <mutex>

// static unordered_map<path_route,std::pair<pathRate,pathWindow>> routeDefinitions;
static std::unordered_map<std::string,std::pair<int,int>> routeDefinitions;

// static std::unordered_map<client_ip,std::unordered_map<path,std::pair<count,first_time>>> clientUsage;
static std::unordered_map<std::string,std::unordered_map<std::string,std::pair<int,time_t>>> clientUsage;

// define mutex
static std::mutex protUsage;

struct limiter
{
    struct context
    {};

    // -- static function creating a map of routes and their respective limits and time window
    static void limit(std::string path , int rateLimit , int windowLimit){
        std::pair<int,int> limit = {rateLimit , windowLimit};
        routeDefinitions[path] = limit;
    }

    // -- main logic function handling --
    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
        std::string path = req.url;

        if(routeDefinitions.count(path)==0) return; // check if url actually exists or not

       	std::string clientIp = req.remote_ip_address;
        int pathRate = routeDefinitions[path].first;
        int pathWindow = routeDefinitions[path].second;
        std::time_t currTime = std::time(nullptr);

        std::lock_guard<std::mutex> lock(protUsage);

        auto& usage = clientUsage[clientIp][path];

        if(usage.second == 0){
            usage.first = 1;
            usage.second = currTime;
        }

        else{
            auto timeDiff = currTime - usage.second;
            if(timeDiff > pathWindow){
                usage.first = 1;
                usage.second = currTime;
            }
            else{
                 ++usage.first;
                if(usage.first > pathRate){
                    res.code = 429;
                    std::cout << "Blocked the ip " << clientIp << std::endl;
                    res.end();
                    return;
                }
            }
        }
    }

    // -- debug functions to print the debug info (will be disabled in prod) --
    static void debug() {
        std::cout << "--- Route Definitions ---\n";
        for (const auto& [path, limits] : routeDefinitions) {
            std::cout << "Path: " << path
                      << ", Rate: " << limits.first
                      << ", Window: " << limits.second << "s\n";
        }

        std::cout << "--- Client Usage  ---\n";
        for (const auto& [client_ip, path_map] : clientUsage) {
            std::cout << "Client IP: " << client_ip << "\n";
            for (const auto& [path, usage] : path_map) {
                std::cout << "  Path: " << path
                          << ", Count: " << usage.first
                          << ", Window Start: " << usage.second << "\n";
            }
        }
        std::cout << "----------------------------\n";
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {}
};

#endif
