#ifndef Dictionary_H
#define Dictionary_H

#include "utility/Vector.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <memory>


class Dictionary
{
    // Private Data

        // Shared pointer keeps the root dictionary from destroyed
        std::shared_ptr<const nlohmann::json> root_;

        // Pointer to the subDict
        const nlohmann::json* node_;

        // Error message
        std::string path_;

    // Private Constructor

        // Construct from components
        Dictionary
        (
            std::shared_ptr<const nlohmann::json> root,
            const nlohmann::json* node,
            std::string path
        )
        :
            root_(std::move(root)),
            node_(node),
            path_(std::move(path))
        {}

    // Private Member Functions

        // Check keys
        bool found(const std::string& key) const
        {
            return node_->contains(key);
        }


public:

    // Check file name before construction
    static Dictionary read(const std::string& fileName)
    {
        std::ifstream file(fileName);
        if (!file) throw std::runtime_error("Cannot open " + fileName);

        auto root = std::make_shared<nlohmann::json>
        (
            nlohmann::json::parse(file, nullptr, true, true)
        );
        const nlohmann::json* node = root.get();

        return Dictionary(std::move(root), node, fileName);
    }

    template<class T>
    T get(const std::string& key) const
    {
        if (!found(key))
            throw std::runtime_error("Missing entry: " + path_ + "/" + key);

        try
        {
            return node_->at(key).get<T>();
        }
        catch(const nlohmann::json::exception&)
        {
            throw std::runtime_error
                ("Wrong type for entry: " + path_ + "/" + key);
        }
    }

    template<class T>
    T getOrDefault(const std::string& key, const T& defaultValue) const
    {
        return found(key) ? get<T>(key) : defaultValue;
    }

    Dictionary subDict(const std::string& key) const
    {
        if (!found(key) || !node_->at(key).is_object())
            throw std::runtime_error("Missing subDict: " + path_ + "/" + key);

        return Dictionary(root_, &node_->at(key), path_ + "/" + key);
    }

    std::vector<std::string> keys() const
    {
        std::vector<std::string> k;
        for (const auto& item : node_->items())
            k.push_back(item.key());
        
        return k;
    }
};


inline void from_json(const nlohmann::json& j, Vector& v)
{
    v.x = j.at(0).get<double>();
    v.y = j.at(1).get<double>();
}


#endif