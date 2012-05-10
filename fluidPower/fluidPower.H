/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    Foam::fluidPower

Description
    Calculates the mechanical energy change across the system (i.e. the work output).

    Member function calcDEmHead() calculates and returns the change in mechanical
    energy across the system and the hydrodynamical head.

    Member function fluidPower::write() calls calcDEmHead() and writes the
    data into the file <time dir>/forces.dat

Author
    Mikko Auvinen

Modifications
    Ported to OpenFoam-1.6-ext using patch by David Bogar - Penn State, ARL

    Added capability to analyse both pumps and turbines using a flag in the 
    controlDict by Bryan Lewis - Penn State 

SourceFiles
    fluidPower.C
    IOfluidPower.H

\*---------------------------------------------------------------------------*/

#ifndef fluidPower_H
#define fluidPower_H

#include "primitiveFieldsFwd.H"
#include "volFieldsFwd.H"
#include "HashSet.H"
#include "Tuple2.H"
#include "OFstream.H"
#include "Switch.H"
#include "pointFieldFwd.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class objectRegistry;
class dictionary;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class forces Declaration
\*---------------------------------------------------------------------------*/

class fluidPower
{
public:
    // Used to be 'pressureViscous'
    // Tuple which contains the work output (.first()) and the hydrodynamic head (.second()) 
    typedef Tuple2<scalar , scalar> dEmHead;  // dEmHead := difference in mechanical energy & Head


/*
    //- Sum operation class to accumulate the pressure, viscous forces and moments
    class sumOp
    {
    public:

        forcesMoments operator()
        (
            const forcesMoments& fm1,
            const forcesMoments& fm2
        ) const
        {
            return forcesMoments
            (
                pressureViscous
                (
                    fm1.first().first() + fm2.first().first(),
                    fm1.first().second() + fm2.first().second()
                ),
                pressureViscous
                (
                    fm1.second().first() + fm2.second().first(),
                    fm1.second().second() + fm2.second().second()
                )
            );
        }
    };
*/

protected:

    // Private data

        //- Name of this set of forces,
        //  Also used as the name of the probes directory.
        word name_;

        const objectRegistry& obr_;

        //- on/off switch
        bool active_;

        //- Switch to send output to Info as well as to file
        Switch log_;

        // Read from dictonary

            //- Fag to determin if pump or turbine (Bryan)
            Switch turbine_;

            //- Patches to integrate the inflow of mechanical energy over
            labelHashSet inletPatchSet_;

            //- Patches to integrate the outflow of mechanical energy over
            labelHashSet outletPatchSet_;

            //- Name of pressure field
            word pName_;

            //- Name of velocity field
            word Uname_;

            //- Name of flux field
            word phiName_;

            //- Reference density needed for incompressible calculations
            scalar rhoRef_;


        //- mechanical energy file ptr
        autoPtr<OFstream> fluidPowerFilePtr_;


    // Private Member Functions

        //- If the fluidPower file has not been created create it
        void makeFile();

        //- Return rhoRef if the pressure field is dynamic, i.e. p/rho
        //  otherwise return 1
        scalar rho(const volScalarField& p) const;

        //- Disallow default bitwise copy construct
        fluidPower(const fluidPower&);

        //- Disallow default bitwise assignment
        void operator=(const fluidPower&);

        //- Output file header information
        virtual void writeFileHeader();


        // Functions to be over-ridden from IOoutputFilter class
       /*
        I'm royally confused by these two virtual functions. Hopefully I'll be able to get a clarification for this.
        - mikko
       */


            //- Update mesh
            virtual void updateMesh(const mapPolyMesh&);

            //- Move points
            virtual void movePoints(const Field<point>&);

public:

    //- Runtime type information
    TypeName("fluidPower");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        fluidPower
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    // Destructor

        virtual ~fluidPower();


    // Member Functions

        //- Return name of the set of fluidPower
        virtual const word& name() const
        {
            return name_;
        }

        //- Read the mechanical energy data
        virtual void read(const dictionary&);

        //- Calculate the work output and write
        virtual void write();

        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Calculate and return the work output and hydrodynamic head
        virtual dEmHead calcDEmHead() const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
