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

#include <string>

#include <sneze/app/config.hpp>
#include <sneze/platform/error.hpp>

namespace sneze {

    class application {
    public:
        inline application( const std::string& team, const std::string& name ):
            team_{ team }, name_{ name }, config_{ team, name } {}

        virtual ~application() = default;

        result<bool, error> run();

        virtual void on_start() = 0;

        virtual void on_end() = 0;

        [[nodiscard]] inline const std::string& team() const noexcept { return team_; }

        [[nodiscard]] inline const std::string& name() const noexcept { return name_; }

        template <is_value Type>
        [[maybe_unused]] inline void
        set_config_value( const std::string& section, const std::string& name, const Type& value ) {
            config_.set_value<Type>( section, name, value );
        }

        template <is_value Type>
        [[maybe_unused]] [[nodiscard]] inline Type
        get_config_value( const std::string& section, const std::string& name, const Type& default_value ) {
            return config_.get_value<Type>( section, name, default_value );
        }

        template <is_value Type>
        [[maybe_unused]] inline void get_set_config_value( const std::string& section,
                                                           const std::string& name,
                                                           const Type& default_value,
                                                           Type ( *func )( Type ) ) {
            Type value = config_.get_value( section, name, default_value );
            value = func( value );
            config_.set_value<Type>( section, name, value );
        }

        template <is_value Type>
        [[maybe_unused]] inline void
        get_set_config_value( const std::string& name, const Type& default_value, Type ( *func )( Type ) ) {
            get_set_config_value<Type>( name_, name, default_value, func );
        }

        template <is_value Type>
        [[maybe_unused]] inline void set_config_value( const std::string& name, const Type& value ) {
            config_.set_value<Type>( name_, name, value );
        }

        template <is_value Type>
        [[maybe_unused]] [[nodiscard]] inline Type get_config_value( const std::string& name,
                                                                     const Type& default_value ) {
            return config_.get_value<Type>( name_, name, default_value );
        }

    protected:
        std::string team_;
        std::string name_;
        config config_;

    private:
        result<> launch() noexcept;

        result<> read_config() noexcept;

        result<> save_config() noexcept;

        [[nodiscard]] const error& show_error( const error& err ) const noexcept;
    };

} // namespace sneze