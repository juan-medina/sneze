/****************************************************************************
MIT License

Copyright (c) 2023 Juan Medina

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

#pragma once

#include <sneze/logger.h>
#include <sneze/result.h>

#include <filesystem>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <toml.hpp>

namespace sneze {

    class config {
    public:
        config( std::string team, std::string application );

        virtual ~config() = default;

        result<bool, error> read();

        typedef std::variant<std::int64_t, double, bool, std::string> config_value;

        template <class Type>
        inline void set_value( const std::string& section, const std::string& name, const Type& value ) {
            if ( !data_.contains( section ) ) { data_[section] = config::section{}; }
            data_[section][name] = value;
        }

        template <class Type>
        inline Type get_value( const std::string& section, const std::string& name, const Type& default_value ) {
            config_value value = default_value;
            if ( data_.contains( section ) ) {
                if ( data_[section].contains( name ) ) {
                    value = data_[section][name];
                    if ( !std::holds_alternative<Type>( value ) ) {
                        LOG_ERR(
                            "error trying to get config value with wrong type, section: {}, value: {}, default to {}",
                            section,
                            name,
                            default_value );
                        value = default_value;
                    }
                }
            }
            set_value( section, name, value );
            return std::get<Type>( value );
        }

        result<bool, error> save();

    protected:
        std::string team_;
        std::string application_;

    private:
        constexpr static const char* const CONFIG_FILE_NAME = "config.toml";

        static result<> exist_or_create_directory( const std::filesystem::path& path ) noexcept;

        static result<> exist_or_create_file( const std::filesystem::path& path ) noexcept;

        result<std::filesystem::path> calculate_config_file_path() noexcept;

        result<> read_toml_config() noexcept;

        typedef std::unordered_map<std::string, config_value> section;
        typedef std::unordered_map<std::string, section> sections;

        sections data_;
        std::filesystem::path config_file_path_;

        result<>
        add_toml_value( const std::string& section, const std::string& name, const toml::value& value ) noexcept;
    };

} // namespace sneze
