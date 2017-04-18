using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop.Interfaces
{
	public interface IRenderer
	{
		int AddTriangle(List<Point> points, Color color, float width);
		int AddQuad(List<Point> points, Color color, float width);
		int AddEllipse(List<Point> points, Color color, float width);

		void ModifyTriangle(int triangleIndex, List<Point> points, Color color, float width);
		void ModifyQuad(int quadIndex, List<Point> points, Color color, float width);
		void ModifyEllipse(int ellipseIndex, List<Point> points, Color color, float width);

		void RenderTriangle(int triangleIndex);
		void RenderQuad(int quadIndex);
		void RenderEllipse(int ellipseIndex);
	}
}
