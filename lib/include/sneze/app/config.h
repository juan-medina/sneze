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

#include <sneze/platform/result.h>
#include <sneze/system/logger.h>

#include <filesystem>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include <toml.hpp>

namespace sneze {

    template <typename T>
    concept is_value = std::is_same_v<T, std::int64_t> || std::is_same_v<T, double> || std::is_same_v<T, bool> ||
                       std::is_same_v<T, std::string>;

    class config {
    public:
        config( std::string team, std::string application );

        virtual ~config() = default;

        result<> read();

        typedef std::variant<std::int64_t, double, bool, std::string> config_value;

        template <is_value Type>
        inline void set_value( const std::string& section, const std::string& name, const Type& value ) {
            if ( auto it_section = data_.find( section ); it_section != data_.end() ) {
                it_section->second[name] = value;
            } else {
                auto new_section = config::section{};
                new_section[name] = value;
                data_[section] = new_section;
            }
        }

        template <is_value Type>
        inline Type get_value( const std::string& section, const std::string& name, const Type& default_value ) {
            if ( auto it_section = data_.find( section ); it_section != data_.end() ) {
                if ( auto it_value = it_section->second.find( name ); it_value != it_section->second.end() ) {
                    if ( auto value = std::get_if<Type>( &it_value->second ) ) { return *value; }
                }
            }
            LOG_DEBUG( "config value not found, section: {}, value: {}, default to {}", section, name, default_value );
            set_value<Type>( section, name, default_value );
            return default_value;
        }

        result<> save();

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
