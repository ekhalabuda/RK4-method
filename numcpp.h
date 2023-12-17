#pragma once

#include <vector>
#include <functional>
#include <cmath>
#include <fstream>


class numcpp {
private:

	inline const static std::pair<double, double> make_RK4_step(double x_n, double v_n, double h, const std::function<double(double, double)>& f) {

		double k1 = f(x_n, v_n);
		double k2 = f(x_n + h / 2, v_n + h * k1 / 2);
		double k3 = f(x_n + h / 2, v_n + h * k2 / 2);
		double k4 = f(x_n + h, v_n + h * k3);

		double x = x_n + h;
		double v = v_n + h * (k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6);

		return {x, v};
	}

public:

    const static std::pair<std::vector<double>, std::vector<double>>
            RK4(double x0, double xn, double u0, std::function<double(double, double)> f,double R , double V , double L,
                double step, double precision, size_t max_step, bool dif_step, double eps) {

            std::ofstream table("Table.txt");
            std::ofstream exact_values("Exact_values.txt");
            std::ofstream final_data("Final_data.txt");

            size_t iters = 0;
            size_t divs = 0;
            size_t doubles = 0;
            size_t divs_ = 0;
            size_t doubles_ = 0;
            double max_olp = 0;
            double max_gp = 0;
            auto u = [ u0, R, V, L](double x) { return (V - (std::exp(-1*x*R/L))*(V - u0*R))/R; };
            std::pair<double, double> max_h(step, 0);
            std::pair<double, double> min_h(step, 0);
            bool excit = false;

            std::vector<double> x_vals{ x0 };
            std::vector<double> v_vals{ u0 };

            double h = step;

            table << iters++ << ' ' << x0 << ' ' << h << ' ' << u0 << ' ' << '-' << ' '
                << '-' << ' ' << '-' << ' ' << divs << ' ' << doubles
                << ' ' << u(x0) << ' ' << abs(u(x0) - u0) << '\n';

            exact_values << u(x0) <<'\n';

            while (iters < max_step && h > 0.00001) {
                if (x_vals.back() + h > xn && x_vals.back() < xn - eps && dif_step){
                    h = xn - x_vals.back();
                    dif_step = false;
                    excit = true;

                }
                else if (x_vals.back() + h > xn )
                    break;
                auto [x, v] = make_RK4_step(x_vals.back(), v_vals.back(), h, f);

                auto [x_half, v_half] = make_RK4_step(x_vals.back(), v_vals.back(), h / 2.0, f);
                auto [x_help, v_help] = make_RK4_step(x_half, v_half, h / 2.0, f);

                size_t p = 4;
                double S = (v_help - v) / (pow(2.0, p) - 1.0);

                //////////////////////////////////////////////////////
                if (dif_step)
                {
                    if (abs(S) > precision) {
                        h /= 2.0;
                        ++divs;
                        continue;
                    }
                    else if (abs(S) < precision / (pow(2.0, p + 1.0))) {
                        h *= 2.0;
                        ++doubles;
                    }
                }
                //////////////////////////////////////////////////////

                double local_p = S * pow(2, p);
                max_olp = abs(local_p) > max_olp? abs(local_p): max_olp;
                max_gp = abs(u(x) - v) > max_gp ? abs(u(x) - v) : max_gp;
                if(h!=0){
                    table << iters << ' ' << x << ' ' << h << ' ' << v << ' ' << v_help << ' '
                        << abs(v_help - v) << ' ' << abs(local_p) << ' ' << divs << ' ' << doubles
                        << ' ' << u(x) << ' ' << abs(u(x) - v) << '\n';
                }


                if(max_h.first < h){
                    max_h.first = h;
                    max_h.second = x;
                }
                if(min_h.first > h && h > 0){
                    min_h.first = h;
                    min_h.second = x;
                }


                exact_values << u(x)<<'\n';
                divs_ += divs;
                doubles_ += doubles;
                ++iters;
                divs = 0;
                doubles = 0;

                x_vals.push_back(x);
                v_vals.push_back(v);
                if(excit){
                   break;
                }

            }


            final_data << --iters << ' ' <<x_vals.back()<<' '<<v_vals.back()<<' '<<xn-x_vals.back() << ' ' << max_olp << ' '
                       << divs_ << ' ' << doubles_ << ' ' << max_h.first << ' ' << max_h.second << ' '
                       << min_h.first << ' ' << min_h.second << ' ' << max_gp<<'\n';

            table.close();
            exact_values.close();
            final_data.close();
            return { x_vals, v_vals };
        }


};


