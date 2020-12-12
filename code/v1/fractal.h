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
        std::complex<double> func(std::complex<double> num)  { return pow(num, 3) - std::complex<double>(1, 0); }
        std::complex<double> deriv(std::complex<double> num) { return std::complex<double>(3, 0)*pow(num, 2);   }

        // zeros and colors
        std::complex<double> zeros [3] = { std::complex<double>(1, 0),
                                           std::complex<double>(-1/2, sqrt(3)/2),
                                           std::complex<double>(-1/2, -sqrt(3)/2) };
       rgb_t colors [3] = { make_colour(210, 0, 0),
                            make_colour(0, 210, 0),
                            make_colour(0, 0, 210) };

        std::complex<double> newtons_method(std::complex<double> num) {
            int cap = 100;
            int i;
            for (i = 0; i < cap; i++) {
                num = num - this->func(num)/this->deriv(num);
            }
            return num;
        }

    public:

        // constructor
        Newton(rgb_t div) { this->div = div; }

        rgb_t color_complex_num(std::complex<double> num) {
            double eps = 0.0000001;
            std::complex<double> zero = this->newtons_method(num);
            int z;
            int index = -1;
            for (z = 0; z < 3; z++) {
                if (zero.real() - this->zeros[z].real() <= eps) {
                    if (zero.imag() - this->zeros[z].imag() <= eps) {
                        index = z;
                    }
                }
            }
            if (index == -1) { return div; }
            else { return this->colors[index]; }
        }

};
