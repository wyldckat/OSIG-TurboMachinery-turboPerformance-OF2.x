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

#include "fluidPower.H"
#include "volFields.H"
#include "surfaceFields.H"    // added
#include "dictionary.H"
#include "Time.H"
#include "incompressible/singlePhaseTransportModel/singlePhaseTransportModel.H"
#include "basicThermo.H"



// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(fluidPower, 0);
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// This function is useful, but it is exactly the same as in 'forces', so I'm repeating code here ...
// ... but not much   -- mikko

Foam::scalar Foam::fluidPower::rho(const volScalarField& p) const
{
    if (p.dimensions() == dimPressure)
    {
        return 1.0;
    }
    else
    {
        return rhoRef_;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Same constructor layout as in 'forces'. The differences lie only in the private data --mikko
Foam::fluidPower::fluidPower
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    name_(name),
    obr_(obr),
    active_(true),
    log_(false),
    turbine_(false),            // Set to true if turbine  (Bryan)
    inletPatchSet_(),           // new labelHashSet
    outletPatchSet_(),        //  -- " --    -- mikko
    pName_(""),
    Uname_(""),
    phiName_(""),
    rhoRef_(0),
    fluidPowerFilePtr_(NULL)       // autoPtr<OFstream> 
{
    // Check if the available mesh is an fvMesh otherwise deactivate
    if (!isA<fvMesh>(obr_))
    {
        active_ = false;
        WarningIn
        (
            "fluidPower::fluidPower(const objectRegistry& obr, const dictionary& dict)"
        )   << "No fvMesh available, deactivating."
            << endl;
    }

    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fluidPower::~fluidPower()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fluidPower::read(const dictionary& dict)
{
    if (active_)
    {
        log_ = dict.lookupOrDefault<Switch>("log", false);

        // Here is an example how to use the dynamic_cast operator 
        // Check src/OpenFOAM/db/typeInfo/typeInfo.H        -- mikko (I'm still a bit fuzzy about this)
        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        // Read if pump or turbine (Bryan)
        turbine_ = dict.lookupOrDefault<Switch>("turbine", false); 

        inletPatchSet_ =
            mesh.boundaryMesh().patchSet(wordReList(dict.lookup("inletPatches")));

        outletPatchSet_ =
            mesh.boundaryMesh().patchSet(wordReList(dict.lookup("outletPatches")));

        // Optional entries U, p, and phi 
        pName_ = dict.lookupOrDefault<word>("pName", "p");
        Uname_ = dict.lookupOrDefault<word>("Uname", "U");
        phiName_ = dict.lookupOrDefault<word>("phiName", "phi");

        // Check whether Uname and pName exists, if not deactivate the function object
        if
        (
            !obr_.foundObject<volVectorField>(Uname_)
         || !obr_.foundObject<volScalarField>(pName_)
         || !obr_.foundObject<surfaceScalarField>(phiName_)
        )
        {
            active_ = false;
            WarningIn("void fluidPower::read(const dictionary& dict)")
                << "Could not find " << Uname_ << " or "
                << pName_ << " or " << phiName_ <<" in database." << nl
                << "    De-activating fluidPower."
                << endl;
        }

        // Reference density needed for incompressible calculations
        rhoRef_ = readScalar(dict.lookup("rhoInf"));

    }
}


void Foam::fluidPower::makeFile()
{
    // Create the forces file if not already created
    if (!fluidPowerFilePtr_.valid())
    {
        if (debug)
        {
            Info<< "Creating file for mechanical power output data." << endl;
        }

        // File update
        if (Pstream::master())
        {
            fileName fluidPowerDir;
            if (Pstream::parRun())
            {
                // Put in undecomposed case (Note: gives problems for
                // distributed data running)
                // Here the 'name_' determines the data directory name  -- mikko
                fluidPowerDir =
                obr_.time().path()/".."/name_/obr_.time().timeName();  
            }
            else
            {
                fluidPowerDir = obr_.time().path()/name_/obr_.time().timeName();
            }

            // Create directory if does not exist.
            mkDir(fluidPowerDir);

            // Open new file at start up
            fluidPowerFilePtr_.reset(new OFstream(fluidPowerDir/(type() + ".dat")));

            // Add headers to output data
            writeFileHeader();
        }
    }
}


void Foam::fluidPower::writeFileHeader()
{
    if (fluidPowerFilePtr_.valid())
    {
        fluidPowerFilePtr_()
            << "# Time" << tab << "  dEm (W) " << tab << " Head (m) " << endl;
    }
}

void Foam::fluidPower::execute()
{
    // Do nothing - only valid on write
}

void Foam::fluidPower::end()
{
    // Do nothing - only valid on write
}

void Foam::fluidPower::timeSet()
{
    // Do nothing - only valid on write
}

void Foam::fluidPower::write()
{
    if (active_)
    {
        // Create the forces file if not already created
        makeFile();

        // Obtain the difference dEm = (Em_output - Em_input) and the hydrodynamic head 
        dEmHead  dEmH = calcDEmHead();

        // It's better to put tabs in between. Printing the entire object dEmH places parenthesis around the values
        // which is royally annoying.    -- mikko
        if (Pstream::master())
        {
            fluidPowerFilePtr_() << obr_.time().value() 
                                 << tab << dEmH.first() << tab << dEmH.second()  << endl;

            if (log_)
            {
                Info<< " Fluid power output:" << nl
                    << "  dEm (W) = " << dEmH.first() << nl
                    << "  Head (m) = " << dEmH.second() << nl
                    << endl;
            }
        }
    }
}


// The computation of dEm and Head
Foam::fluidPower::dEmHead Foam::fluidPower::calcDEmHead() const
{
    const volVectorField& U       = obr_.lookupObject<volVectorField>(Uname_);
    const volScalarField& p       = obr_.lookupObject<volScalarField>(pName_);
    const surfaceScalarField& phi = obr_.lookupObject<surfaceScalarField>(phiName_);

    // const fvMesh& mesh = U.mesh();

    dEmHead dEmH( scalar(0) , scalar(0) );

    const volVectorField::GeometricBoundaryField& Ub = U.boundaryField();
    const volScalarField::GeometricBoundaryField& pb = p.boundaryField();
	  const surfaceScalarField::GeometricBoundaryField&  phib = phi.boundaryField(); 


    scalar mflowInlet  = 0.0;        // mass flow into the domain
    scalar mflowOutlet = 0.0;        // mass flow out of the domain (currently not needed)
    scalar EmInlet     = 0.0;       // Mechanical energy flow into the domain (in Watts!)
    scalar EmOutlet    = 0.0;       // Mechanical energy flow out of the domain

    // inflow of mechanical energy
    forAllConstIter(labelHashSet, inletPatchSet_, iter)
    {
        label patchi = iter.key();

        scalarField ptInlet  =  pb[patchi] + 0.5*magSqr(Ub[patchi]);
       
        mflowInlet  += rho(p)* sum( phib[patchi]) + VSMALL;    // Sign: Inflow (-)
        EmInlet     += rho(p)* sum( phib[patchi] * ptInlet );
    }

    // outflow of mechanical energy
    forAllConstIter(labelHashSet, outletPatchSet_, iter)
    {
        label patchi = iter.key();

        scalarField ptOutlet  =  pb[patchi] + 0.5*magSqr(Ub[patchi]);
            
        mflowOutlet += rho(p)* sum( phib[patchi]) + VSMALL;   // - Signs: Outflow(+) 
        EmOutlet    += rho(p)* sum( phib[patchi] * ptOutlet );
    }


    reduce(EmInlet , sumOp<scalar>());
    reduce(EmOutlet , sumOp<scalar>());
    reduce(mflowInlet , sumOp<scalar>());

    //dEmH.first() = (EmOutlet + EmInlet );                // The rate of work output from the system (W)
    //dEmH.second() = (EmOutlet + EmInlet )/(-mflowInlet*scalar(9.81));  // Hydrodynamic (total) head

    if (turbine_) // (Bryan)
    {
        dEmH.first() = scalar(-1.0)*(EmOutlet + EmInlet );                // The rate of work output from the system (W)
        dEmH.second() = scalar(-1.0)*(EmOutlet + EmInlet )/(-mflowInlet*scalar(9.81));  // Hydrodynamic (total) head
    }
    else
    {
       dEmH.first() = (EmOutlet + EmInlet );                // The rate of work output from the system (W)
       dEmH.second() = (EmOutlet + EmInlet )/(-mflowInlet*scalar(9.81));  // Hydrodynamic (total) head
    }
	
    return dEmH;
}

// ************************************************************************* //
