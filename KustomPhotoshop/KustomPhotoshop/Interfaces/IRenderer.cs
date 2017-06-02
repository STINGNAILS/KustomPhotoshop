using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public interface IRenderer
	{
		int AddPolyLine(List<Point> points, Color color, float width);
		int AddTriangle(List<Point> points, Color color, Color borderColor, float width);
		int AddQuad(List<Point> points, Color color, Color borderColor, float width);
		int AddEllipse(Point o, float aO, float bO, float angle, Color color, Color borderColor, float width);

		void ModifyPolyLine(int polyLineIndex, List<Point> points, Color color, float width);
		void ModifyTriangle(int triangleIndex, List<Point> points, Color color, Color borderColor, float width);
		void ModifyQuad(int quadIndex, List<Point> points, Color color, Color borderColor, float width);
		void ModifyEllipse(int ellipseIndex, Point o, float aO, float bO, float angle, Color color, Color borderColor, float width);
		void ModifyBoundingBox(List<Point> points);

		void RenderPolyLine(int polyLineIndex);
		void RenderTriangle(int triangleIndex);
		void RenderQuad(int quadIndex);
		void RenderEllipse(int ellipseIndex);
		void RenderBoundingBox();

		void RemovePolyLine(int polyLineIndex);
        void RemoveTriangle(int triangleIndex);
        void RemoveQuad(int quadIndex);
        void RemoveEllipse(int ellipseIndex);
    }
}
