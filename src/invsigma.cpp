/**
 * \file   invsigma.cpp
 * \author mkooyman
 *
 * \brief Contains the implementation of the InvSigma class.
 */
/*
 *
 * Copyright (C) 2009--2015 Various members of the GenABEL team. See
 * the SVN commit logs for more details.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 *
 */


#include <string>
#include <sstream>
#include <fstream>

#include "fvlib/AbstractMatrix.h"
#include "fvlib/CastUtils.h"
#include "fvlib/const.h"
#include "fvlib/convert_util.h"
#include "fvlib/FileVector.h"
#include "fvlib/frutil.h"
#include "fvlib/frversion.h"
#include "fvlib/Logger.h"
#include "fvlib/Transposer.h"
#include "invsigma.h"
#include "phedata.h"
#include "gendata.h"
#include "eigen_mematrix.h"
#include "eigen_mematrix.cpp"
#include "utilities.h"


InvSigma::InvSigma(const char * filename_, const phedata& phe) : filename(filename_)
{
    npeople = phe.nids;
    std::ifstream myfile(filename_);
    char * line = new char[MAXIMUM_PEOPLE_AMOUNT];
    std::string id;

    matrix.reinit(npeople, npeople);

    // idnames[k], if (allmeasured[i]==1)

    if (myfile.is_open())
    {
        double val;
        unsigned row = 0;
        while (myfile.getline(line, MAXIMUM_PEOPLE_AMOUNT))
        {
            std::stringstream line_stream(line);
            line_stream >> id;

            if (phe.idnames[row] != id)
            {
                std::cerr << "error:in row " << row << " id="
                          << phe.idnames[row]
                          << " in inverse variance matrix but id=" << id
                          << " must be there. Wrong inverse variance matrix"
                          << " (only measured id must be there)\n";
                exit(1);
            }
            unsigned col = 0;
            while (line_stream >> val)
            {
                matrix.put(val, row, col);
                col++;
            }

            if (col != npeople)
            {
                std::cerr << "error: inv file: Number of columns in row "
                          << row << " equals to " << col
                          << " but number of people is " << npeople << "\n";
                myfile.close();
                exit(1);
            }
            row++;
        }
        myfile.close();
    } else {
        std::cerr << "error: inv file: cannot open file '"
                  << filename_ << "'\n";
    }

    delete[] line;
}


InvSigma::~InvSigma()
{
}


mematrix<double> & InvSigma::get_matrix(void)
{
    return matrix;
}
