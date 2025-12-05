#pragma once

#include <fstream>
#include <windows.h>

#include "json.hpp"

#define CONFIG_FILE_NAME "config.json"

using json = nlohmann::json;

struct AgeClass {
    std::string name;
    int max_age;
};

struct AdultClass {
    std::string male;
    std::string female;
};

struct SexClass {
    std::string male;
    std::string female;
};

struct Config {
    std::vector<std::string> typeAPrefixes;
    std::vector<std::string> typeBPrefixes;

    std::vector<AgeClass> ageClasses;
    AdultClass adult;
    SexClass sex;

    std::string weightSuffix;
};

Config loadConfig(const std::string& filename) {

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    std::filesystem::path configPath = exeDir / filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Konnte config.json nicht öffnen!");
    }

    json j;
    file >> j;

    Config cfg;

    // Prefixes
    cfg.typeAPrefixes = j["input_types"]["A"]["prefixes"].get<std::vector<std::string>>();
    cfg.typeBPrefixes = j["input_types"]["B"]["prefixes"].get<std::vector<std::string>>();

    // Age classes (dynamic)
    for (auto& ac : j["kids_classes"]) {
        cfg.ageClasses.push_back({
            ac["name"].get<std::string>(),
            ac["max_age"].get<int>()
            });
    }

    // Adult classes
    cfg.adult.male = j["adult_class"]["male"].get<std::string>();
    cfg.adult.female = j["adult_class"]["female"].get<std::string>();

    // Geschlecht
    cfg.sex.male = j["sex"]["male"].get<std::string>();
    cfg.sex.female = j["sex"]["female"].get<std::string>();

    // Weight suffix
    cfg.weightSuffix = j["weight_suffix"].get<std::string>();

    return cfg;
}


Config cfg = loadConfig(CONFIG_FILE_NAME);