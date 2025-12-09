
#ifndef _LIMITER_H
#define _LIMITER_H

#include "crow.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <mutex>

static std::unordered_map<std::string,std::pair<int,int>> routeDefinitions;
// static unordered_map<path_route,std::pair<pathRate,pathWindow>> routeDefinitions;

static std::unordered_map<std::string,std::unordered_map<std::string,std::pair<int,time_t>>> clientUsage;
// static std::unordered_map<client_ip,std::unordered_map<path,std::pair<count,first_time>>> clientUsage;

static std::mutex protUsage;

struct limiter
{
    struct context
    {};

    static void limit(std::string path , int ratelimit , int windowlimit){
        std::pair<int,int> limit = {ratelimit , windowlimit};
        routeDefinitions[path] = limit;
    }

    void before_handle(crow::request& req, crow::response& res, context& ctx)
    {
    	std::string client_ip = req.remote_ip_address;
        std::string path = req.url;
        int pathRate = routeDefinitions[path].first - 1;
        int pathWindow = routeDefinitions[path].second;
        std::time_t now = std::time(nullptr);

        std::lock_guard<std::mutex> lock(protUsage);

        auto& usage = clientUsage[client_ip][path];

        if(usage.second == 0){
            usage.first = 1;
            usage.second = now;
        }

        else{
            auto difference = now - usage.second;
            if(difference > pathWindow){
                usage.first = 0;
                usage.second = now;
            }
            else{
                if(usage.first > pathRate){
                    res.code = 429;
                    std::cout << "Blocked the ip " << client_ip << std::endl;
                    res.end();
                }
                else ++usage.first;
            }
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx)
    {}
};

#endif
