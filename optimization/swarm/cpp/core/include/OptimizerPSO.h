
#include <OptimizerInterface.h>


using namespace std;


namespace optimization
{

    struct PSOparticle
    {
        Vec pos;
        Vec vel;
        double cost;

        Vec bestPos;
        Vec bestCost;

        PSOparticle( int ndim )
        {
            this->pos = Vec::zeros( ndim );
            this->vel = Vec::zeros( ndim );
            this->cost = 0.0;

            this->bestPos = Vec::zeros( ndim );
            this->bestCost = 0.0;
        }
    };



    class OptimizerPSO : public OptimizerInterface
    {

        private :

        int m_populationSize;

        vector< PSOparticle > m_particles;
        PSOparticle m_bestParticle;

        double m_w;
        double m_c1;
        double m_c2;
        double m_k;

        public :

        OptimizerPSO( int pNdim, 
                      double pDomainMin, double pDomainMax, int pMaxIter, 
                      double w, double c1, double c2, double k );
        ~OptimizerPSO();

        void start() override;
        void step() override;
        void end() override;

        // Accessors

        vector< PSOparticle >& particles() { return m_particles; }

    };






}