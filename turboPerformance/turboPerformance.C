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

\*---------------------------------------------------------------------------*/

#include "turboPerformance.H"
#include "dictionary.H"
#include "Time.H"
#include "Pstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(turboPerformance, 0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::turboPerformance::turboPerformance
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    forces(name, obr, dict, loadFromFiles),
    fluidPower(name, obr, dict, loadFromFiles),
    omega_(vector::zero)

{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::turboPerformance::~turboPerformance()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void Foam::turboPerformance::execute()
{
    // Do nothing - only valid on write
}

void Foam::turboPerformance::end()
{
    // Do nothing - only valid on write
}

void Foam::turboPerformance::read(const dictionary& dict)
{
    if (fluidPower::active_)
    {
        forces::read(dict);
        fluidPower::read(dict);

    // For now omega (in rad/s) is the only additional info we need 
        dict.lookup("omega") >> omega_;  

    }
}


void Foam::turboPerformance::writeFileHeader()
{
    if ( fluidPowerFilePtr_.valid() )  // The data directory will be named after 'name_' ... see fluidPower::makeFile().
    {
        fluidPowerFilePtr_()
            << "# Time" << tab << "Head (m)" <<tab << "Axial Power (W)" << tab << "Efficiency (%)" << tab
             << "F_x" << tab <<"F_y" << tab <<"F_z" << endl;
    }
}


void Foam::turboPerformance::write()
{
    if (fluidPower::active_)
    {
        // Create the data file if not already created
        fluidPower::makeFile();

        forcesMoments fm = forces::calcForcesMoment();
        dEmHead     dEmH  =  fluidPower::calcDEmHead();

        vector totForce = fm.first().first() + fm.first().second();
        vector totMoment = fm.second().first() + fm.second().second();

        scalar TOmega = fabs( totMoment & omega_);        // Axial power (W)
        scalar eff = ( dEmH.first() / TOmega ) * scalar(100);  // Pump Efficiency (%)

        if (fluidPower::turbine_) // Turbine flag from fluidPower (Bryan)
        {	
             eff = ( TOmega / dEmH.first() ) * scalar(100);  // Turbine Efficiency (%)
        }

        // Tab separated output ... to avoid those irritating parenthesis.       -- mikko
        if (Pstream::master())
        {
            fluidPowerFilePtr_()
                << fluidPower::obr_.time().value() << tab << dEmH.second() <<tab << TOmega << tab << eff << tab
                << totForce[0] << tab << totForce[1] << tab <<totForce[2] << endl;

            if (fluidPower::log_)
            {
                Info<< " performance data:" << nl
                    << "   Head (m)   = " << dEmH.second()  << nl
                    << "   TOmega (W) = " << TOmega << nl
                    << "   Eff (%)    = " << eff << nl
                    << "   Forces     = " << totForce << nl
                    << "   Moments    = " << totMoment << nl
                    << endl;
            }
        }
    }
}

void Foam::turboPerformance::updateMesh(const mapPolyMesh&)
{
    // Do nothing
}

void Foam::turboPerformance::movePoints(const pointField&)
{
    // Do nothing
}

// ************************************************************************* //
