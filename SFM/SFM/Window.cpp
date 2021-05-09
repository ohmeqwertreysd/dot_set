// Файл Window.cpp, который определяет методы класса Window, а также вспомогательные функции
#include "Window.h"
bool operator==(const floatw& r, const floatw& l)
{
    return fabs(r.data - l.data) <= epsilon_float;
}

bool operator<(const floatw& r, const floatw& l)
{
    return r == l ? r.data < l.data : r.data < l.data;
}

float operator*(const floatw& r, const float& l)
{
    return r.data * l;
}

bool operator==(const sf::Vector2f& left, const sf::Vector2f& right)
{
    return floatw{ left.x } == floatw{ right.x } && floatw{ left.y } == floatw{ right.y };
}

bool operator==(const sf::VertexArray& left, const sf::VertexArray& right)
{
    size_t count = 0;
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            if (left[i].position == right[j].position)
                count++;
    return count == 4;
}

// Вернуть левую верхнюю позицию фигуры
sf::Vector2f get_position_circle(const sf::Vector2f& point)
{
    float x = point.x - dot_size;  // X = X от левого верхнего угла + ширина/2
    float y = point.y - dot_size; // Y = Y от левого верхнего угла + высота/2
    return sf::Vector2f(x, y);
}

float distance_between_two_point(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

sf::Vector2f get_vector_between_two_point(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return { (b.x - a.x), (b.y - a.y) };
}

bool isCollinear(const ds::Pair<sf::Vector2f, sf::Vector2f>& ab, const ds::Pair<sf::Vector2f, sf::Vector2f>& cd)
{
    sf::Vector2f vector_AB = get_vector_between_two_point(ab.first, ab.second);
    sf::Vector2f vector_CD = get_vector_between_two_point(cd.first, cd.second);

    float n = floatw{ vector_AB.x } == floatw{ 0.0 } ? vector_CD.y / vector_AB.y : vector_CD.x / vector_AB.x;

    return sf::Vector2f(n * vector_AB.x, n * vector_AB.y) == vector_CD;
}

bool isOrthogonal(const ds::Pair<sf::Vector2f, sf::Vector2f>& ab, const ds::Pair<sf::Vector2f, sf::Vector2f>& cd)
{
    sf::Vector2f vector_AB{ ab.second.x - ab.first.x, ab.second.y - ab.first.y };
    sf::Vector2f vector_CD{ cd.second.x - cd.first.x, cd.second.y - cd.first.y };

    return floatw{ vector_AB.x * vector_CD.x + vector_AB.y * vector_CD.y } == floatw{ 0.0 };
}

ds::List<sf::VertexArray>* Window::search_square(hashtable* ht, ds::List<floatw>* dists)
{

    ds::List<sf::VertexArray>* squares = new  ds::List<sf::VertexArray>;
    for (size_t i = 0; i < dists->size(); i++)
    {
        ds::List<ds::Pair<sf::Vector2f, sf::Vector2f>>* points = ht->equal_range(dists->operator[](i));

        for (size_t j = 0; j + 1 < points->size(); j++)
        {
            for (size_t k = j + 1; k < points->size(); k++)
            {
                sf::Vector2f a = (*points)[j].first;
                sf::Vector2f b = (*points)[j].second;

                sf::Vector2f c = (*points)[k].first;
                sf::Vector2f d = (*points)[k].second;
                sf::VertexArray line(sf::LineStrip, 5);
                sf::Color line_color = sf::Color::White;
                line[0].color = line_color;
                line[1].color = line_color;
                line[2].color = line_color;
                line[3].color = line_color;
                line[4].color = line_color;
                if (isCollinear({ a,b }, { c,d }) && isCollinear({ a,d }, { c,b }) && isOrthogonal({ a,b }, { a,d }))
                {
                    if (!(floatw(distance_between_two_point(a, b)) == floatw(distance_between_two_point(a, d))))
                        continue;
                    line[0].position = a;
                    line[1].position = b;
                    line[2].position = c;
                    line[3].position = d;
                    line[4].position = a;
                }
                else if (isCollinear({ a,c }, { b,d }) && isCollinear({ a,d }, { c,b }) && isOrthogonal({ a,b }, { c,d }))
                {
                    if (!(floatw(distance_between_two_point(a, c)) == floatw(distance_between_two_point(a, d))))
                        continue;
                    line[0].position = a;
                    line[1].position = c;
                    line[2].position = b;
                    line[3].position = d;
                    line[4].position = a;
                }
                else
                    continue;

                if (squares->find(line) == -1)
                    squares->push_back(line);
            }
        }
    }
    return squares;
}

ds::List<sf::Vector2f>* Window::generate_points(const size_t& dot_amount)
{
    ds::List<sf::Vector2f>* dots = new ds::List<sf::Vector2f>;

    for (size_t i = 0; i < dot_amount; i++)
        dots->push_back(get_random_point());

    return dots;
}

ds::Pair<ds::List<floatw>*, hashtable*> Window::distation_calc()
{
    size_t n = (dots_amount - 1) * dots_amount / 2;
    ds::List<floatw>* dists = new ds::List<floatw>;
    hashtable* hs = new hashtable(n * 2);
    size_t ind = 0U;
    for (int i = 0; i < dots_amount - 1; i++)
    {
        for (int j = i + 1; j < dots_amount; j++, ind++)
        {
            floatw dis = distance_between_two_point(dots->operator[](i), dots->operator[](j));
            if (dists->find(dis) == -1)
                dists->push_back(dis);
            hs->insert({ dis, { dots->operator[](i), dots->operator[](j) } });
        }
    }
    return { dists, hs };
}

// Вернуть рандомную целочисленно/вещественную точку
sf::Vector2f Window::get_random_point()
{
    float x = rand() % (field_width - size_t(dot_size) - field_indent) + float(field_indent) + dot_size;
    float y = rand() % (field_height - size_t(dot_size) - field_indent) + float(field_indent) + dot_size;
    return { x, y };
}

Window::Window() : dots_amount(100), draw_flag(false)
{
    dots = generate_points(dots_amount);;
    dots_textbox = dots_to_textbox();
    squares = new ds::List<sf::VertexArray>;
}

Window::~Window()
{
    delete dots;
    delete squares;
}

void Window::logic()
{
    // ImGuiWindowFlags_NoMove - не перемещать, ImGuiWindowFlags_NoResize - не менять размер, ImGuiWindowFlags_NoCollapse - не сворачивать
    ImGui::Begin("Customization", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse); // создаём окно
    ImGui::SetWindowPos({ 1020.f, float(field_indent) });
    ImGui::SetWindowSize({ 260.f, float(field_height) });

    if (ImGui::InputInt("Amount", &dots_amount))
    {
        dots_amount = dots_amount < 0 ? 0 : dots_amount>100 ? 100 : dots_amount;
    }

    ImGui::Checkbox("Draw Points and Squares", &draw_flag);

    if (ImGui::Button("Generate random Points"))
    {
        delete dots;
        delete squares;
        squares = new ds::List<sf::VertexArray>;
        dots = generate_points(dots_amount);
        dots_textbox = dots_to_textbox();
    }

    if (ImGui::Button("Generate Points from File"))
        dots_textbox = file_to_textbox();
    if (ImGui::Button("Save Points to File"))
        dots_to_file();

    if (ImGui::Button("Generate Points from TextBox"))
        textbox_to_dots();

    if (ImGui::InputTextMultiline("Points (x;y)\nx=R{15..1005}\ny=R{15..705}", const_cast<char*>(dots_textbox.data()), dots_textbox.size() + 255, ImVec2(150, 500)))
    {
        dots_textbox.resize(dots_textbox.size() + 1);
    }

    if (ImGui::Button("Clear Points and Squares"))
    {
        delete dots;
        dots = new ds::List<sf::Vector2f>;
        delete squares;
        squares = new ds::List<sf::VertexArray>;
        dots_textbox.clear();
        dots_amount = 0;
    }

    ImGui::End(); // end window


    ImGui::Begin("Output", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowPos({ 1280.f, float(field_indent) });
    ImGui::SetWindowSize({ 210.f, float(field_height) });
    if (ImGui::Button("Finding and drawing Squares"))
    {
        dots_amount = dots->size();
        delete squares;
        ds::Pair<ds::List<floatw>*, hashtable*> ae = distation_calc();
        squares = search_square(ae.second, ae.first);
        delete ae.first;
        delete ae.second;
    }
    if (ImGui::Button("Save Squares to File"))
        square_to_file();
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Found squares: %d", squares->size());
    for (size_t i = 0; i < squares->size(); i++)
        ImGui::Text("%d: A(%.1lf,%.1lf)\n\tB(%.1lf,%.1lf)\n\tC(%.1lf,%.1lf)\n\tD(%.1lf,%.1lf)", i + 1, squares->operator[](i)[0].position.x, squares->operator[](i)[0].position.y
            , squares->operator[](i)[1].position.x, squares->operator[](i)[1].position.y, squares->operator[](i)[2].position.x, squares->operator[](i)[2].position.y
            , squares->operator[](i)[3].position.x, squares->operator[](i)[3].position.y);
    ImGui::End(); // end window


}

void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::RectangleShape field({ float(field_width), float(field_height) });
	field.setFillColor(sf::Color::Color(160, 160, 235));
	field.setPosition(float(field_indent), float(field_indent));

    



	target.draw(field, states);
    if (draw_flag)
    {
        for (size_t i = 0; i < dots->size(); i++)
        {
            sf::CircleShape dot(dot_size);
            dot.setPosition(get_position_circle(dots->operator[](i)));
            dot.setFillColor(sf::Color::Black);
            target.draw(dot, states);
        }

        for (size_t i = 0; i < squares->size(); i++)
        {
            target.draw((*squares)[i], states);
        }
    }
}

void Window::textbox_to_dots()
{
    delete dots;
    dots = new ds::List<sf::Vector2f>;

    delete squares;
    squares = new ds::List<sf::VertexArray>;


    std::istringstream ss(dots_textbox);
    std::string str;
    while (ss >> str)
    {
        std::istringstream points(str);
        sf::Vector2f dot;
        char del;
        points >> dot.x >> del >> dot.y;
        dots->push_back(dot);
    }
    dots_amount = dots->size();
}

std::string Window::file_to_textbox()
{
    delete dots;
    dots = new ds::List<sf::Vector2f>;

    delete squares;
    squares = new ds::List<sf::VertexArray>;
    std::ifstream in("txt/input_dots.txt");
    std::string str;
    if (in.is_open())
    {
        std::string line;
        while (std::getline(in, line))
        {
            std::istringstream points(line);
            sf::Vector2f dot;
            char del;
            points >> dot.x >> del >> dot.y;
            dots->push_back(dot);
        }
    }
    in.close();
    dots_amount = dots->size();
    return dots_to_textbox();
}

std::string Window::dots_to_textbox()
{
    std::string textbox = "";
    for (int i = 0; i < dots_amount; i++)
        textbox += std::to_string(dots->operator[](i).x) + ";" + std::to_string(dots->operator[](i).y) + "\n";
    return textbox;
}

void Window::dots_to_file()
{
    std::ofstream out("txt/output_dots.txt");
    if (out.is_open())
    {
        for (int i = 0; i < dots_amount; i++)
            out << std::to_string(dots->operator[](i).x) << ";" << std::to_string(dots->operator[](i).y) << std::endl;
    }
    out.close();
}

void Window::square_to_file()
{
    std::ofstream out("txt/squares.txt");
    if (out.is_open())
    {
        out << "Found squares: " << squares->size() << std::endl;
        for (size_t i = 0; i < squares->size(); i++)
            out << std::to_string(i + 1) <<
            ":\n\tA(" << squares->operator[](i)[0].position.x << ", " << squares->operator[](i)[0].position.y <<
            ")\n\tB(" << squares->operator[](i)[1].position.x << ", " << squares->operator[](i)[1].position.y <<
            ")\n\tC(" << squares->operator[](i)[2].position.x << ", " << squares->operator[](i)[2].position.y <<
            ")\n\tD(" << squares->operator[](i)[3].position.x << ", " << squares->operator[](i)[3].position.y << ")\n";
    }
    out.close();
}