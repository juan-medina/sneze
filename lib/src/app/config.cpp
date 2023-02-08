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

#include "sneze/app/config.h"

#include <filesystem>
#include <fstream>
#include <utility>

#include "platform_folders.h"
#include "sneze/system/logger.h"

namespace sneze {

    config::config( std::string team, std::string application ):
        team_{ std::move( team ) }, application_{ std::move( application ) } {}

    namespace fs = std::filesystem;

    result<> config::read() {
        LOG_INFO( "Reading config for application: {} (Team: {})", application_, team_ );

        if ( auto [val, err] = calculate_config_file_path().ok(); err ) {
            LOG_ERR( "error calculate config file path" );
            return error( "Can't calculate config file path.", *err );
        } else {
            LOG_DEBUG( "Config file: {}", val->string() );
            config_file_path_ = *val;
        }

        if ( auto err = read_toml_config().ko() ) {
            LOG_ERR( "error reading toml file: {}", config_file_path_.string() );
            return error( "Can't read config file.", *err );
        }

        LOG_INFO( "config file read" );
        return true;
    }

    result<std::filesystem::path> config::calculate_config_file_path() noexcept {
        auto home = sago::getConfigHome();

        // home path
        fs::path home_folder( home );
        if ( !fs::exists( home_folder ) ) {
            LOG_ERR( "error finding home path: {}", home );
            return error( "Can't get game config directory." );
        }

        // team path
        fs::path team_path( team_ );
        fs::path team_full_path = home_folder / team_path;
        if ( auto err = exist_or_create_directory( team_full_path ).ko() ) {
            LOG_ERR( "error checking team path: {}", team_full_path.string() );
            return error( "Can't get game config directory.", *err );
        }

        // application path
        fs::path application_path( application_ );
        fs::path application_full_path = team_full_path / application_path;
        if ( auto err = exist_or_create_directory( application_full_path ).ko() ) {
            LOG_ERR( "error checking application path: {}", team_full_path.string() );
            return error( "Can't get game config directory.", *err );
        }

        // config file
        fs::path config_file( CONFIG_FILE_NAME );
        fs::path config_file_full_path = application_full_path / config_file;
        if ( auto err = exist_or_create_file( config_file_full_path ).ko() ) {
            LOG_ERR( "error checking config file: {}", config_file_full_path.string() );
            return error( "Can't find or create config file.", *err );
        }

        return config_file_full_path;
    }

    result<> config::exist_or_create_directory( const std::filesystem::path& path ) noexcept {
        if ( fs::exists( path ) ) {
            LOG_WARN( "directory already exist: {}", path.string() );
        } else {
            LOG_DEBUG( "Creating directory: {}", path.string() );
            std::error_code ec;
            fs::create_directory( path, ec );
            std::error_condition ok;
            if ( ec != ok ) {
                LOG_ERR( "error creating directory: {}", ec.message() );
                return error( "Can't get config directory." );
            }
            if ( !fs::exists( path ) ) {
                LOG_ERR( "directory does no exists after creating: {}", path.string() );
                return error( "Can't get config directory." );
            }
        }
        return true;
    }

    result<> config::exist_or_create_file( const std::filesystem::path& path ) noexcept {
        if ( fs::exists( path ) ) {
            LOG_WARN( "file already exist: {}", path.string() );
        } else {
            LOG_DEBUG( "Creating empty file: {}", path.string() );
            std::fstream file;
            file.open( path, std::ios::out );
            if ( file.fail() ) {
                LOG_ERR( "failed to open file: {}", path.string() );
                return error( "Can't create empty config file." );
            }
            file.close();
            if ( file.fail() ) {
                LOG_ERR( "failed to close file: {}", path.string() );
                return error( "Can't create empty config file." );
            }
            if ( !fs::exists( path ) ) {
                LOG_ERR( "file does no exists after creating: {}", path.string() );
                return error( "Can't create empty config file." );
            }
        }
        return true;
    }

    result<> config::read_toml_config() noexcept {
        LOG_INFO( "reading : {}", config_file_path_.string() );

        try {
            auto data = toml::parse( config_file_path_ );
            for ( const auto& [section, section_value] : data.as_table() ) {
                if ( section_value.is_table() ) {
                    for ( const auto& [name, value] : section_value.as_table() ) {
                        if ( auto err = add_toml_value( section, name, value ).ko(); err ) {
                            LOG_ERR( "Error parsing toml data " );
                            return error( "Error reading config file." );
                        }
                    }
                }
            }
        } catch ( toml::exception& toml_exception ) {
            const auto msg = toml_exception.what();
            const auto& location = toml_exception.location();
            LOG_ERR( "toml exception: {}, reading config file {} ({},{})",
                     msg,
                     location.file_name(),
                     location.line(),
                     location.column() );
            return error( "Invalid config file." );
        } catch ( std::runtime_error& runtime_error ) {
            const auto msg = runtime_error.what();
            LOG_ERR( "exception reading config file: {} ", msg );
            return error( "Error reading config file." );
        }

        return true;
    }

    result<>
    config::add_toml_value( const std::string& section, const std::string& name, const toml::value& value ) noexcept {
        switch ( value.type() ) {
        case toml::value_t::boolean:
            set_value( section, name, value.as_boolean() );
            break;
        case toml::value_t::integer:
            set_value( section, name, value.as_integer() );
            break;
        case toml::value_t::floating:
            set_value( section, name, value.as_floating() );
            break;
        case toml::value_t::string:
            set_value( section, name, value.as_string().str );
            break;
        default:
            LOG_ERR( "toml value no supported for value: {}, in section {}", name, section );
            return error( "Invalid config file." );
        }
        return true;
    }

    result<> config::save() {
        LOG_INFO( "Saving config to: {}", config_file_path_.string() );

        auto toml_data = toml::basic_value<toml::preserve_comments>();

        for ( auto [section_name, table] : data_ ) {
            auto section = toml::value();
            for ( auto [value_name, value] : table ) {
                if ( std::holds_alternative<bool>( value ) ) {
                    section[value_name] = std::get<bool>( value );
                } else if ( std::holds_alternative<double>( value ) ) {
                    section[value_name] = std::get<double>( value );
                } else if ( std::holds_alternative<std::int64_t>( value ) ) {
                    section[value_name] = std::get<std::int64_t>( value );
                } else if ( std::holds_alternative<std::string>( value ) ) {
                    section[value_name] = std::get<std::string>( value );
                }
            }
            toml_data[section_name] = section;
        }

        std::ofstream file;
        file.open( config_file_path_ );
        if ( file.fail() ) {
            LOG_ERR( "failed to open file: {}", config_file_path_.string() );
            return error( "Can't save config file." );
        }

        file << "# generated by sneze" << std::endl << std::setw( 0 ) << toml_data << std::endl;
        if ( file.fail() ) {
            LOG_ERR( "failed to write to file: {}", config_file_path_.string() );
            return error( "Can't save config file." );
        }

        file.close();
        if ( file.fail() ) {
            LOG_ERR( "failed to close file: {}", config_file_path_.string() );
            return error( "Can't save config file." );
        }
        LOG_INFO( "Config saved" );
        return true;
    }

} // namespace sneze