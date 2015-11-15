
#ifndef RHIZOME_OHMBREWER_PID_PROFILE_H
#define RHIZOME_OHMBREWER_PID_PROFILE_H

namespace Ohmbrewer {

    class PIDProfile {

    public:

        /**
         * Constructor
         * @param p P term
         * @param i I term
         * @param d D term
         */
        PIDProfile(double p, double i, double d) {
            _kP = p;
            _kI = i;
            _kD = d;
        }

        /**
         * Getter for the P term
         * @returns P
         */
        const double kP() { return _kP; }

        /**
         * Getter for the I term
         * @returns I
         */
        const double kI() { return _kI; }

        /**
         * Getter for the D term
         * @returns D
         */
        const double kD() { return _kD; }

    protected:
        /**
         * Setting for the P term
         */
        double _kP;

        /**
         * Setting for the I term
         */
        double _kI;

        /**
         * Setting for the D term
         */
        double _kD;
    };
};
#endif
