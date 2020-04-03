#ifndef KAKERA_ENGINE_CUBIC_BEZIER
#define KAKERA_ENGINE_CUBIC_BEZIER

#include <vector>

class CubicBezier
{
public:
    struct Point
    {
        Point(float _x, float _y) : x(_x), y(_y) {}
        float x, y;
    };
private:
    std::vector<Point> control_points;
    int steps = 0;

    void calculate()
    {
        results.emplace_back(0.0f, 0.0f);

        float dx1 = control_points[0].x;
        float dy1 = control_points[0].y;
        float dx2 = control_points[1].x - control_points[0].x;
        float dy2 = control_points[1].y - control_points[0].y;
        float dx3 = 1.0f - control_points[1].x;
        float dy3 = 1.0f - control_points[1].y;

        float subdiv_step = 1.0f / (steps + 3);
        float subdiv_step2 = subdiv_step * subdiv_step;
        float subdiv_step3 = subdiv_step * subdiv_step * subdiv_step;

        float pre1 = 3.0 * subdiv_step;
        float pre2 = 3.0 * subdiv_step2;
        float pre4 = 6.0 * subdiv_step2;
        float pre5 = 6.0 * subdiv_step3;

        float tmp1x = 0.0f - control_points[0].x * 2.0f + control_points[1].x;
        float tmp1y = 0.0f - control_points[0].y * 2.0f + control_points[1].y;

        float tmp2x = (control_points[0].x - control_points[1].x) * 3.0f + 1.0f;
        float tmp2y = (control_points[0].y - control_points[1].y) * 3.0f + 1.0f;

        float fx = 0.0f;
        float fy = 0.0f;

        float dfx = control_points[0].x * pre1 + tmp1x * pre2 + tmp2x * subdiv_step3;
        float dfy = control_points[0].y * pre1 + tmp1y * pre2 + tmp2y * subdiv_step3;

        float ddfx = tmp1x * pre4 + tmp2x * pre5;
        float ddfy = tmp1y * pre4 + tmp2y * pre5;

        float dddfx = tmp2x * pre5;
        float dddfy = tmp2y * pre5;

        for (int i = 0; i < steps; i++) {
            fx += dfx;
            fy += dfy;
            dfx += ddfx;
            dfy += ddfy;
            ddfx += dddfx;
            ddfy += dddfy;
            results.emplace_back(fx, fy);
        }

        results.emplace_back(1.0f, 1.0f);
    }
public:
    std::vector<Point> results;

    CubicBezier(float x1 = 0.0f, float y1 = 0.0f, float x2 = 1.0f, float y2 = 1.0f)
    {
        control_points.emplace_back(x1, y1);
        control_points.emplace_back(x2, y2);
    }

    CubicBezier(const CubicBezier& other)
    {
        steps = other.steps;
        control_points = other.control_points;
        results = other.results;
    }

    CubicBezier(CubicBezier&& other)
    {
        steps = other.steps;
        other.steps = 0;
        control_points = std::move(other.control_points);
        results = std::move(other.results);
    }

    ~CubicBezier() = default;

    CubicBezier& operator=(const CubicBezier& other)
    {
        if (this != &other)
        {
            steps = other.steps;
            control_points = other.control_points;
            results = other.results;
        }
        return *this;
    }

    CubicBezier& operator=(CubicBezier&& other)
    {
        if (this != &other)
        {
            steps = other.steps;
            other.steps = 0;
            control_points = std::move(other.control_points);
            results = std::move(other.results);
        }
        return *this;
    }

    void set_steps(int steps)
    {
        this->steps = steps - 2;
        calculate();
    }
};

#endif // !KAKERA_ENGINE_CUBIC_BEZIER
