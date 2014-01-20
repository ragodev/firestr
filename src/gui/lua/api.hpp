/*
 * Copyright (C) 2014  Maxim Noah Khailo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either vedit_refsion 3 of the License, or
 * (at your option) any later vedit_refsion.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FIRESTR_APP_LUA_SCRIPT_API_H
#define FIRESTR_APP_LUA_SCRIPT_API_H

#include "gui/lua/base.hpp"
#include "gui/lua/widgets.hpp"
#include "session/session_service.hpp"

#include <QImage>

namespace fire
{
    namespace gui
    {
        namespace lua
        {
            using script_ptr = std::shared_ptr<SLB::Script>;
            using widget_map = std::unordered_map<int, QWidget*>;
            using image_map = std::unordered_map<int, std::shared_ptr<QImage>>;
            using layout_map = std::unordered_map<int, QGridLayout*>;
            using timer_map = std::unordered_map<int, QTimer*>;

            struct error_info
            {
                int line;
                std::string message;
            };
            using error_infos = std::vector<error_info>;

            class lua_api : public QObject
            {
                Q_OBJECT
                public:
                    lua_api(
                            const user::contact_list& con,
                            messages::sender_ptr sender,
                            session::session_ptr session,
                            session::session_service_ptr session_service,
                            QWidget* can,
                            QGridLayout* lay,
                            list* out = nullptr);

                    ~lua_api();

                public:
                    //misc
                    error_infos errors; 
                    list* output;
                    QWidget* canvas;
                    QGridLayout* layout;
                    SLB::Manager manager;
                    script_ptr state;
                    std::mutex mutex;


                    //message
                    user::contact_list contacts;
                    session::session_ptr session;
                    session::session_service_ptr session_service;
                    messages::sender_ptr sender;
                    std::string message_callback;
                    std::string local_message_callback;

                    //util
                    void bind();
                    error_info execute(const std::string&);
                    void run(const std::string&);
                    void reset_widgets();
                    void message_recieved(const script_message&);
                    void send_to_helper(user::user_info_ptr, const script_message&); 

                    button_ref_map button_refs;
                    label_ref_map label_refs;
                    edit_ref_map edit_refs;
                    text_edit_ref_map text_edit_refs;
                    list_ref_map list_refs;
                    grid_ref_map grid_refs;
                    draw_ref_map draw_refs;
                    timer_ref_map timer_refs;
                    image_ref_map image_refs;

                    //all widgets referenced are stored here
                    layout_map layouts;
                    widget_map widgets;
                    image_map images;
                    timer_map timers;

                    //id functions
                    int ids;
                    int new_id();

                    //error func
                    void report_error(const std::string& e, int line = -1);
                    error_info get_error() const;

                    //================================
                    //API
                    //================================

                    //gui
                    void print(const std::string& a);
                    void alert();
                    button_ref make_button(const std::string& title);
                    label_ref make_label(const std::string& text);
                    edit_ref make_edit(const std::string& text);
                    text_edit_ref make_text_edit(const std::string& text);
                    list_ref make_list();
                    draw_ref make_draw(int width, int height);
                    QPen make_pen(const std::string& color, int width);
                    timer_ref make_timer(int msec, const std::string& callback);
                    image_ref make_image(const bin_data& data);

                    grid_ref make_grid();
                    void place(const widget_ref& w, int r, int c);
                    void place_across(const widget_ref& w, int r, int c, int row_span, int col_span);
                    void height(int h);
                    void grow();

                    //messages
                    void set_message_callback(const std::string& a);
                    void set_local_message_callback(const std::string& a);

                    script_message make_message();
                    void send_all(const script_message&);
                    void send_to(const contact_ref&, const script_message&); 
                    void send_local(const script_message& m);

                    //contacts
                    size_t total_contacts() const;
                    int last_contact() const;
                    contact_ref get_contact(size_t);

                    //apps
                    size_t total_apps() const;
                    app_ref get_app(size_t);

                    //file
                    file_data open_file();
                    bin_file_data open_bin_file();
                    bool save_file(const std::string& suggested_name, const std::string& data);
                    bool save_bin_file(const std::string& suggested_name, const bin_data& data);

                    public slots:
                        void button_clicked(int id);
                        void edit_edited(int id);
                        void edit_finished(int id);
                        void text_edit_edited(int id);
                        void timer_triggered(int id);
                private:
                        error_info _error;
            };

            using lua_api_ptr = std::shared_ptr<lua_api>;

            template<class W>
                W* get_widget(int id, widget_map& map)
                {
                    auto wp = map.find(id);
                    return wp != map.end() ? dynamic_cast<W*>(wp->second) : nullptr;
                }
        }
    }
}

#endif

