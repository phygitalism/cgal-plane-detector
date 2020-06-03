#ifdef VTK_VIS

#include "vtk_vis.h"

void viz::show_ply(const std::string & path_to_file, const std::string & orig_file)
{
    vtkSmartPointer<vtkPLYReader> reader1 =
        vtkSmartPointer<vtkPLYReader>::New();
    reader1->SetFileName(path_to_file.c_str());

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper1 =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper1->SetInputConnection(reader1->GetOutputPort());

    vtkSmartPointer<vtkActor> actor1 =
        vtkSmartPointer<vtkActor>::New();
    actor1->SetMapper(mapper1);

    vtkSmartPointer<vtkPLYReader> reader2 =
        vtkSmartPointer<vtkPLYReader>::New();
    reader2->SetFileName(orig_file.c_str());

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper2 =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper2->SetInputConnection(reader2->GetOutputPort());

    vtkSmartPointer<vtkActor> actor2 =
        vtkSmartPointer<vtkActor>::New();
    actor2->SetMapper(mapper2);

    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkSmartPointer<vtkAxesActor> axes =
        vtkSmartPointer<vtkAxesActor>::New();

    actor2->GetProperty()->SetOpacity(0.5);
    actor1->GetProperty()->SetColor(0, 1, 0);
    actor1->GetProperty()->SetPointSize(3);

    renderer->AddActor(actor1);
    renderer->AddActor(actor2);
    renderer->SetBackground(0, 0, 0);
    renderer->UseShadowsOff();
    renderer->AddActor(axes);
    renderWindow->Render();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    renderWindowInteractor->SetInteractorStyle(style);

    renderWindowInteractor->Start();
}

#endif