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
            int iter_cap = 500;                                                 // set an iteration cap
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

        /*rgb_t color_complex_num(std::complex<double> num){
            double eps = 0.0000000001;
            std::complex<double> z = num;                                       // start the input
            std::complex<double> prev = z;                                      // set up previous value
            int iter_cap = 500;                                                 // set an iteration cap
            int i;
            double diff = 1;
            for (i = 0; i < iter_cap && diff > eps; i++) {                      // iterate 0 on z_n+1 = num^z_n
                z = pow(num, z);                                                // exponentiate
                diff = abs(z - prev);                                           // compute the difference between the magnitudes of iterations
                prev = z;                                                       // update previous value
            }
            if (diff <= eps) { return this->bckgrnd; }                          // if orbit has not diverged to infinity, return the background color
            else {                                                              // otherwise, compute the scaled color
                double div = iter_cap/(double)i;
                div = 1000;
                int r = (int) 255*(this->r + (1/div)*(1-this->r));
                int g = (int) 255*(this->g + (1/div)*(1-this->g));
                int b = (int) 255*(this->b + (1/div)*(1-this->b));
                rgb_t color = make_colour(r, g, b);
                return color;
            }
        }*/
};
