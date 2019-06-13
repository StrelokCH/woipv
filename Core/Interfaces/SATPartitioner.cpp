#include "Core/stdafx.h"
#include "SATPartitioner.h"

void SATPartitioner::SetPartitionSolver(std::shared_ptr<SATSolver> solver)
{
    partitionSolver = solver;
}
