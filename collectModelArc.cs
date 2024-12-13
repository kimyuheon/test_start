using Autodesk.Revit.DB;
using Autodesk.Revit.UI;
using System.Collections.Generic;

public class CollectAndConvertToModelArc : IExternalCommand
{
    public Result Execute(
        ExternalCommandData commandData,
        ref string message,
        ElementSet elements)
    {
        UIApplication uiApp = commandData.Application;
        UIDocument uiDoc = uiApp.ActiveUIDocument;
        Document doc = uiDoc.Document;

        // ModelArc 객체를 저장할 리스트
        List<ModelArc> modelArcs = new List<ModelArc>();

        // 1. FilteredElementCollector로 OST_Lines 객체 필터링
        FilteredElementCollector collector = new FilteredElementCollector(doc)
            .OfCategory(BuiltInCategory.OST_Lines)
            .WhereElementIsNotElementType();

        // 2. Transaction을 사용하여 ModelArc로 변환
        using (Transaction trans = new Transaction(doc, "Convert Circles to ModelArcs"))
        {
            trans.Start();

            foreach (Element element in collector)
            {
                // 3. 각 요소의 기하학적 데이터 가져오기
                GeometryElement geometryElement = element.get_Geometry(new Options());

                if (geometryElement == null)
                    continue;

                foreach (GeometryObject geoObject in geometryElement)
                {
                    // 4. Curve가 Arc이고 원형인지 확인
                    if (geoObject is Curve curve && curve is Arc arc && arc.IsCyclic)
                    {
                        // 5. ModelArc 생성
                        SketchPlane sketchPlane = SketchPlane.Create(doc, Plane.CreateByNormalAndOrigin(XYZ.BasisZ, arc.Center));
                        ModelArc modelArc = doc.FamilyCreate.NewModelCurve(arc, sketchPlane) as ModelArc;

                        if (modelArc != null)
                        {
                            modelArcs.Add(modelArc); // 리스트에 추가
                        }
                    }
                }
            }

            trans.Commit();
        }

        // 결과 메시지 표시
        TaskDialog.Show("Result", $"{modelArcs.Count} ModelArcs were created and added to the list.");

        return Result.Succeeded;
    }
}
