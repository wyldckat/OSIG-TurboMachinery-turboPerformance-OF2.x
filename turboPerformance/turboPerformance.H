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
    Foam::turboPerformance

Description
    

SourceFiles
    turboPerformance.C
    IOturboPerformance.H

Authors
    Mikko Auvinen

Modifications
    Ported to OpenFoam-1.6-ext using patch by David Bogar - Penn State, ARL

    Added capability to analyse both pumps and turbines using a flag in the 
    controlDict by Bryan Lewis - Penn State    

\*---------------------------------------------------------------------------*/

#ifndef turboPerformance_H
#define turboPerformance_H

#include "forces.H"
#include "fluidPower.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                        Class turboPerformance Declaration
\*---------------------------------------------------------------------------*/

class turboPerformance
:
    public forces,
    public fluidPower
{
    // Private data

           // - Rotational velocity (in rad/s)
            vector omega_;

    // Private member functions

        //- Disallow default bitwise copy construct
        turboPerformance(const turboPerformance&);

        //- Disallow default bitwise assignment
        void operator=(const turboPerformance&);


protected:

    //- Output file header information
    virtual void writeFileHeader();

        // Functions to be over-ridden from IOoutputFilter class

            //- Update mesh
            virtual void updateMesh(const mapPolyMesh&);

            //- Move points
            virtual void movePoints(const Field<point>&);

public:

    //- Runtime type information
    TypeName("turboPerformance");


    // Constructors

        //- Construct for given objectRegistry and dictionary.
        //  Allow the possibility to load fields from files
        turboPerformance
        (
            const word& name,
            const objectRegistry&,
            const dictionary&,
            const bool loadFromFiles = false
        );


    // Destructor

        virtual ~turboPerformance();


    // Member Functions


        //- Execute, currently does nothing
        virtual void execute();

        //- Execute at the final time-loop, currently does nothing
        virtual void end();

        //- Read the forces data
        virtual void read(const dictionary&);

        //- Calculate the forces and write
        virtual void write();


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
