#define HAS_VTK 1
#define _IS_DEBUG 1

#include "vtkPointData.h"
#include <vtkPointPicker.h>
#include <vtkCommand.h>
#include <vtkMarchingCubes.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkMarchingCubes.h>
#include <vtkVectorNorm.h>
#include <vtkDataSetMapper.h>
#include <vtkImageToPolyDataFilter.h>
#include <vtkPolyDataReader.h>
#include <vtkLookupTable.h>
#include <vtkSphereSource.h>
#include <vtkCallbackCommand.h>
#include <vtkProperty.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkPolyDataWriter.h>
#include <vtkCellData.h>
#include <vtkPolyDataReader.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkMaskPoints.h>
#include <vtkFloatArray.h>
#include <vtkPointLocator.h>


#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


  
/*
*   For each vertex in target, gets the closest vertex on the source post-transformation, and copies the point ID 
*   Writes the target polygon with Point IDs from source as output 
*/
void Compare_VTK(char* source_poly_fn, char* target_poly_fn, char* output_txt, char* output_poly_fn) 
{
  
  double xyz_target[3];
  int closestPointID; 
  vector<double> target_scalars_hit;
  vector<double> source_scalars_list;
  double target_scalar, source_scalar;

  vtkSmartPointer<vtkPolyData> source_poly =vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPolyData> target_poly =vtkSmartPointer<vtkPolyData>::New();  
  
  // to search for closest point on polyWithColors for transfering the colors from that shell 
    
  vtkSmartPointer<vtkPolyDataReader> reader1 = vtkSmartPointer<vtkPolyDataReader>::New(); 
  reader1->SetFileName(source_poly_fn); 
  reader1->Update();
  source_poly = reader1->GetOutput();

  vtkSmartPointer<vtkPolyDataReader> reader2 = vtkSmartPointer<vtkPolyDataReader>::New(); 
  reader2->SetFileName(target_poly_fn); 
  reader2->Update();
  target_poly = reader2->GetOutput();

  // To search for closest points in target polygon 
  vtkSmartPointer<vtkPointLocator> point_locator_in_target = vtkSmartPointer<vtkPointLocator>::New();
  point_locator_in_target->SetDataSet(target_poly); 
  point_locator_in_target->AutomaticOn(); 
  point_locator_in_target->BuildLocator();

  int* vertex_visited_in_target = new int[target_poly->GetNumberOfPoints()];
  vtkSmartPointer<vtkFloatArray> target_scalar_to_indicate_visited = vtkSmartPointer<vtkFloatArray>::New();


  for (vtkIdType i = 0; i < target_poly->GetNumberOfPoints(); ++i) {
      vertex_visited_in_target[i] = 0;      // not visited yet 
      target_scalar_to_indicate_visited->InsertNextTuple1(0);          // to mark the polydata with points that were visited
  }

  vtkSmartPointer<vtkFloatArray> target_scalars = vtkSmartPointer<vtkFloatArray>::New();
  target_scalars = vtkFloatArray::SafeDownCast(target_poly->GetPointData()->GetScalars());

   vtkSmartPointer<vtkFloatArray> source_scalars = vtkSmartPointer<vtkFloatArray>::New();
  source_scalars = vtkFloatArray::SafeDownCast(source_poly->GetPointData()->GetScalars());
  

  
  // iterating through each point in source to find closest target point hit  
  for (vtkIdType i = 0; i < source_poly->GetNumberOfPoints(); ++i) {
      
      source_poly->GetPoint(i, xyz_target); 
      closestPointID = point_locator_in_target->FindClosestPoint(xyz_target);

      if (closestPointID > 0 && closestPointID < target_poly->GetNumberOfPoints())
      {
          if (vertex_visited_in_target[closestPointID] <= 0)
          {
              vertex_visited_in_target[closestPointID] = 1;
              target_scalar = target_scalars->GetTuple1(closestPointID);
              target_scalars_hit.push_back(target_scalar);
              target_scalar_to_indicate_visited->SetTuple1(closestPointID, 1);

              // recording the source scalar 
              source_scalar = source_scalars->GetTuple1(i);
              source_scalars_list.push_back(source_scalar);

              
          } 
      }


  }

  target_poly->GetPointData()->SetScalars(target_scalar_to_indicate_visited);
  
  // writing target scalars that were hit to file 
  ofstream output; 
  output.open(output_txt);
  output << "Source point,Target point,Difference (S-T)\n";
  output << "============,============,================\n";

  for (int i=0;i<target_scalars_hit.size();i++)
  {
      double diff = fabs(source_scalars_list[i]-target_scalars_hit[i]);
      if (source_scalars_list[i] > 0)
        output <<  source_scalars_list[i] << "," << target_scalars_hit[i] << "," << diff  << endl; 
  }

  output.close();

  vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
  writer->SetInputData(target_poly);
  writer->SetFileName(output_poly_fn);
  writer->Update();

}

int main(int argc, char **argv)
{
  
  char* target_poly_fn, *source_poly_fn, *output_poly_fn, *output_txt_fn;
   
  if (argc != 5)
  {
     cerr << "Not enough parameters\nUsage: <source.vtk> <target.vtk> <output.csv> <output.vtk>"<< endl; 
     exit(1);
  }
  
  source_poly_fn = argv[1];
  target_poly_fn = argv[2];
  output_poly_fn = argv[3];
  output_txt_fn = argv[4];
 

  
	Compare_VTK(source_poly_fn, target_poly_fn, output_poly_fn, output_txt_fn);  
  
  
}