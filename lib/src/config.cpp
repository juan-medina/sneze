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

#include <filesystem>
#include <fstream>
#include <utility>

#include <platform_folders.h>
#include <sneze/config.h>
#include <sneze/logger.h>

namespace sneze {

    config::config( std::string team, std::string application ):
        team_{ std::move( team ) }, application_{ std::move( application ) } {
    }

    namespace fs = std::filesystem;

    result<bool, error> config::read() {
        LOG_DEBUG( "Reading config for application: {} (Team: {})", application_, team_ );

        if ( auto [val, err] = calculate_config_file_path().check(); err ) {
            LOG_ERR( "error calculate config file path" );
            return error( "Can't calculate config file path.", *err );
        } else {
            LOG_DEBUG( "Config file: {}", val->generic_string() );
        }

        return true;
    }

    result<std::filesystem::path, error> config::calculate_config_file_path() {
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
        if ( auto [val, err] = exist_or_create_directory( team_full_path ).check(); err ) {
            LOG_ERR( "error checking team path: {}", team_full_path.generic_string() );
            return error( "Can't get game config directory.", *err );
        }

        // application path
        fs::path application_path( application_ );
        fs::path application_full_path = team_full_path / application_path;
        if ( auto [val, err] = exist_or_create_directory( application_full_path ).check(); err ) {
            LOG_ERR( "error checking application path: {}", team_full_path.generic_string() );
            return error( "Can't get game config directory.", *err );
        }

        // config file
        fs::path config_file( CONFIG_FILE_NAME );
        fs::path config_file_full_path = application_full_path / config_file;
        if ( auto [val, err] = exist_or_create_file( config_file_full_path ).check(); err ) {
            LOG_ERR( "error checking config file: {}", config_file_full_path.generic_string() );
            return error( "Can't find or create config file.", *err );
        }

        return config_file_full_path;
    }

    result<bool, error> config::exist_or_create_directory( const std::filesystem::path& path ) noexcept {
        if ( fs::exists( path ) ) {
            LOG_WARN( "directory already exist: {}", path.generic_string() );
        } else {
            LOG_DEBUG( "Creating directory: {}", path.generic_string() );
            std::error_code ec;
            fs::create_directory( path, ec );
            std::error_condition ok;
            if ( ec != ok ) {
                LOG_ERR( "error creating directory: {}", ec.message() );
                return error( "Can't get config directory." );
            }
            if ( !fs::exists( path ) ) {
                LOG_ERR( "directory does no exists after creating: {}", path.generic_string() );
                return error( "Can't get config directory." );
            }
        }
        return true;
    }

    result<bool, error> config::exist_or_create_file( const std::filesystem::path& path ) noexcept {
        if ( fs::exists( path ) ) {
            LOG_WARN( "file already exist: {}", path.generic_string() );
        } else {
            LOG_DEBUG( "Creating empty file: {}", path.generic_string() );
            std::fstream file;
            file.open( path, std::ios::out );
            if ( file.fail() ) {
                LOG_ERR( "failed to open file: {}", path.generic_string() );
                return error( "Can't create config file." );
            }
            file.close();
            if ( file.fail() ) {
                LOG_ERR( "failed to close file: {}", path.generic_string() );
                return error( "Can't create config file." );
            }
            if ( !fs::exists( path ) ) {
                LOG_ERR( "file does no exists after creating: {}", path.generic_string() );
                return error( "Can't create config file." );
            }
        }
        return true;
    }

} // namespace sneze