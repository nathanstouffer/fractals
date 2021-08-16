// interface IFractalGen
// with method color_complex_num()

/* Abstract class that generates a fractal by returning the appropriate color of a
 * complex input
*/
class FractalGen {
    public:

        //FractalGen() {};
        //~FractalGen() {};

        virtual rgb_t color_complex_num(std::complex<double> num) = 0;
};

/* class that colors a complex number c according to the iterative rule z_n+1 = z_n + c
 * where z_0 = 0
 */
class Mandelbrot : public FractalGen {

    private:

        rgb_t conv;
        double r, g, b;

    public:

        // constructor
        Mandelbrot(rgb_t conv, double r=1, double g=1, double b=1) {
            this->conv = conv;
            this->r = r;
            this->g = g;
            this->b = b;
        }

        rgb_t color_complex_num(std::complex<double> num){
            // quick check to decrease computation time
            if (abs(num) < 0.2) { return this->conv; }
            else if (num.real() < 0){
                if (abs(num) < 0.6) { return this->conv; }
            }
            std::complex<double> z(0.0, 0.0);                                   // start the 0-orbit
            int cap = 500;                                                      // set an iteration cap
            int i;
            for (i = 0; i < cap && abs(z) <= 2; i++) {                          // iterate 0 on z_n+1 = z_n^2 + num
                z = pow(z, 2) + num;
            }
            if (abs(z) <= 2) { return this->conv; }                             // if orbit has not diverged to infinity, return the background color
            else {                                                              // otherwise, compute the scaled color
                double div = cap/(double)i;
                int r = (int) 255*(this->r + (1/div)*(1-this->r));
                int g = (int) 255*(this->g + (1/div)*(1-this->g));
                int b = (int) 255*(this->b + (1/div)*(1-this->b));
                rgb_t color = make_colour(r, g, b);
                return color;
            }
        }
};


/* class that colors a complex number c according to the iterative rule z_n+1 = num^z_n
 * where z_0 = num
 */
class PowerTower : public FractalGen {

    private:

        rgb_t conv;
        double r, g, b;

    public:

        // constructor
        PowerTower(rgb_t conv, double r=1, double g=1, double b=1) {
            this->conv = conv;
            this->r = r;
            this->g = g;
            this->b = b;
        }

        rgb_t color_complex_num(std::complex<double> num){
            std::complex<double> z = num;                                       // start the input
            std::complex<double> prev = z;                                      // set up previous value
            int iter_cap = 200;                                                 // set an iteration cap
            double mag_cap = 50;                                                // set up a magnitude cap
            int i;
            for (i = 0; i < iter_cap && abs(z) < mag_cap; i++) {                // iterate 0 on z_n+1 = num^z_n
                z = pow(num, z);                                                // exponentiate
            }
            if (abs(z) < mag_cap) { return this->conv; }                        // if orbit has not diverged to infinity, return the background color
            else {                                                              // otherwise, compute the scaled color
                double div = iter_cap/(double)i;
                div = 1000;
                int r = (int) 255*(this->r + (1/div)*(1-this->r));
                int g = (int) 255*(this->g + (1/div)*(1-this->g));
                int b = (int) 255*(this->b + (1/div)*(1-this->b));
                rgb_t color = make_colour(r, g, b);
                return color;
            }
        }

};

/* class that colors a complex number c according to newton's method for finding zeros
 * of a function
 */
class Newton : public FractalGen {

    private:

        rgb_t div;
        std::complex<double> func(std::complex<double> z)  { return std::complex<double>(1, 0)*pow(z, 4)
                                                                  + std::complex<double>(0, 0)*pow(z, 3)
                                                                  + std::complex<double>(0, 0)*pow(z, 2)
                                                                  + std::complex<double>(0, 0)*z
                                                                  + std::complex<double>(-1, 0); }

        std::complex<double> deriv(std::complex<double> z) { return std::complex<double>(4, 0)*pow(z, 3)
                                                                  + std::complex<double>(0, 0)*pow(z, 2)
                                                                  + std::complex<double>(0, 0)*z
                                                                  + std::complex<double>(0, 0);   }

        // zeros and colors
        std::complex<double> zeros [4] = { std::complex<double>(-1, 0),
                                           std::complex<double>(1, 0),
                                           std::complex<double>(0, -1),
                                           std::complex<double>(0, 1) };
       rgb_t colors [4] = { make_colour(0, 5, 30),
                            make_colour(0, 10, 170),
                            make_colour(36, 70, 255),
                            make_colour(0, 10, 70) };

        std::complex<double> newtons_method(std::complex<double> num, double eps) {
            std::complex<double> prev;
            int cap = 100;
            int i;
            for (i = 0; i < cap; i++) {
                prev = num;
                num = num - this->func(num)/this->deriv(num);
                if (abs(num - prev) <= eps) { return num; }
            }
            return num;
        }

        // method to return the index of the zeros array within eps (a small value)
        int index(std::complex<double> num, double eps) {
            int index = -1;
            int length = sizeof(this->zeros)/sizeof(this->zeros[0]);
            int z;
            for (z = 0; z < length; z++) {
                if (abs(num - this->zeros[z]) <= eps) { index = z; }
            }
            return index;
        }

    public:

        // constructor
        Newton(rgb_t div) { this->div = div; }

        rgb_t color_complex_num(std::complex<double> num) {
            double eps = 0.000000001;
            std::complex<double> zero = this->newtons_method(num, eps);
            int index = this->index(zero, eps);
            if (index == -1) { return div; }
            else { return this->colors[index]; }
        }

};
