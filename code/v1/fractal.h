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

        rgb_t bckgrnd;
        double r, g, b;

    public:

        // constructor
        Mandelbrot(rgb_t bckgrnd, double r=1, double g=1, double b=1) {
            this->bckgrnd = bckgrnd;
            this->r = r;
            this->g = g;
            this->b = b;
        }

        rgb_t color_complex_num(std::complex<double> num){
            std::complex<double> z(0.0, 0.0);                                   // start the 0-orbit
            int cap = 1000;                                                     // set an iteration cap
            int i;
            for (i = 0; i < cap && abs(z) <= 2; i++) {                          // iterate 0 on z_n+1 = z_n^2 + num
                z = pow(z, 2) + num;
            }
            if (abs(z) <= 2) { return this->bckgrnd; }                          // if orbit has not diverged to infinity, return the background color
            else {                                                              // otherwise, compute the scaled color
                double scalar = 1/(double)i;
                //scalar = 1/(abs(z)-2);
                int r = (int) std::min(255*scalar*this->r, 255.0);
                int g = (int) std::min(255*scalar*this->g, 255.0);
                int b = (int) std::min(255*scalar*this->b, 255.0);
                rgb_t color = make_colour(r, g, b);
                return color;
            }
        }
};
