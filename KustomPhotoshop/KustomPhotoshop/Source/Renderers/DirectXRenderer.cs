using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace KustomPhotoshop
{
	public class DirectXRenderer : IRenderer
	{
		public int AddPolyLine(List<Point> points, Color color, float width)
		{
			float x1 = (float)points[0].X;
			float y1 = (float)points[0].Y;
			float x2 = (float)points[1].X;
			float y2 = (float)points[1].Y;
			float x3 = points.Count > 2 ? (float)points[2].X : 0.0f;
			float y3 = points.Count > 2 ? (float)points[2].Y : 0.0f;
			float x4 = points.Count > 3 ? (float)points[3].X : 0.0f;
			float y4 = points.Count > 3 ? (float)points[3].Y : 0.0f;
			float x5 = points.Count > 4 ? (float)points[4].X : 0.0f;
			float y5 = points.Count > 4 ? (float)points[4].Y : 0.0f;
			float x6 = points.Count > 5 ? (float)points[5].X : 0.0f;
			float y6 = points.Count > 5 ? (float)points[5].Y : 0.0f;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			return DirectXDll.AddPolyLine(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, points.Count, r, g, b, width);
		}


		public int AddTriangle(List<Point> points, Color color, Color borderColor, float width)
		{
			float x1 = (float)points[0].X;
			float y1 = (float)points[0].Y;
			float x2 = (float)points[1].X;
			float y2 = (float)points[1].Y;
			float x3 = (float)points[2].X;
			float y3 = (float)points[2].Y;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			float rB = borderColor.R / 255.0f;
			float gB = borderColor.G / 255.0f;
			float bB = borderColor.B / 255.0f;

			return DirectXDll.AddTriangle(x1, y1, x2, y2, x3, y3, r, g, b, rB, gB, bB, width);
		}


		public int AddQuad(List<Point> points, Color color, Color borderColor, float width)
		{
			float x1 = (float)points[0].X;
			float y1 = (float)points[0].Y;
			float x2 = (float)points[1].X;
			float y2 = (float)points[1].Y;
			float x3 = (float)points[2].X;
			float y3 = (float)points[2].Y;
			float x4 = (float)points[3].X;
			float y4 = (float)points[3].Y;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			float rB = borderColor.R / 255.0f;
			float gB = borderColor.G / 255.0f;
			float bB = borderColor.B / 255.0f;

			return DirectXDll.AddQuad(x1, y1, x2, y2, x3, y3, x4, y4, r, g, b, rB, gB, bB, width);
		}


		public int AddEllipse(Point o, float aO, float bO, float angle, Color color, Color borderColor, float width)
		{
			float x = (float)o.X;
			float y = (float)o.Y;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			float rB = borderColor.R / 255.0f;
			float gB = borderColor.G / 255.0f;
			float bB = borderColor.B / 255.0f;

			return DirectXDll.AddEllipse(x, y, aO, bO, angle, r, g, b, rB, gB, bB, width);
		}


		public void ModifyPolyLine(int polyLineIndex, List<Point> points, Color color, float width)
		{
			float x1 = (float)points[0].X;
			float y1 = (float)points[0].Y;
			float x2 = (float)points[1].X;
			float y2 = (float)points[1].Y;
			float x3 = points.Count > 2 ? (float)points[2].X : 0.0f;
			float y3 = points.Count > 2 ? (float)points[2].Y : 0.0f;
			float x4 = points.Count > 3 ? (float)points[3].X : 0.0f;
			float y4 = points.Count > 3 ? (float)points[3].Y : 0.0f;
			float x5 = points.Count > 4 ? (float)points[4].X : 0.0f;
			float y5 = points.Count > 4 ? (float)points[4].Y : 0.0f;
			float x6 = points.Count > 5 ? (float)points[5].X : 0.0f;
			float y6 = points.Count > 5 ? (float)points[5].Y : 0.0f;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			DirectXDll.ModifyPolyLine(polyLineIndex, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6, points.Count, r, g, b, width);
		}


		public void ModifyTriangle(int triangleIndex, List<Point> points, Color color, Color borderColor, float width)
		{
			float x1 = (float)points[0].X;
			float y1 = (float)points[0].Y;
			float x2 = (float)points[1].X;
			float y2 = (float)points[1].Y;
			float x3 = (float)points[2].X;
			float y3 = (float)points[2].Y;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			float rB = borderColor.R / 255.0f;
			float gB = borderColor.G / 255.0f;
			float bB = borderColor.B / 255.0f;

			DirectXDll.ModifyTriangle(triangleIndex, x1, y1, x2, y2, x3, y3, r, g, b, rB, gB, bB, width);
		}


		public void ModifyQuad(int quadIndex, List<Point> points, Color color, Color borderColor, float width)
		{
			float x1 = (float)points[0].X;
			float y1 = (float)points[0].Y;
			float x2 = (float)points[1].X;
			float y2 = (float)points[1].Y;
			float x3 = (float)points[2].X;
			float y3 = (float)points[2].Y;
			float x4 = (float)points[3].X;
			float y4 = (float)points[3].Y;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			float rB = borderColor.R / 255.0f;
			float gB = borderColor.G / 255.0f;
			float bB = borderColor.B / 255.0f;

			DirectXDll.ModifyQuad(quadIndex, x1, y1, x2, y2, x3, y3, x4, y4, r, g, b, rB, gB, bB, width);
		}


		public void ModifyEllipse(int ellipseIndex, Point o, float aO, float bO, float angle, Color color, Color borderColor, float width)
		{
			float x = (float)o.X;
			float y = (float)o.Y;

			float r = color.R / 255.0f;
			float g = color.G / 255.0f;
			float b = color.B / 255.0f;

			float rB = borderColor.R / 255.0f;
			float gB = borderColor.G / 255.0f;
			float bB = borderColor.B / 255.0f;

			DirectXDll.ModifyEllipse(ellipseIndex, x, y, aO, bO, angle, r, g, b, rB, gB, bB, width);
		}


		public void ModifyBoundingBox(List<Point> points)
		{
			float xMin = (float)points[0].X;
			float yMin = (float)points[0].Y;
			float xMax = (float)points[1].X;
			float yMax = (float)points[1].Y;

			DirectXDll.ModifyBoundingBox(xMin, yMin, xMax, yMax);
		}


		public void RenderPolyLine(int polyLineIndex)
		{
			DirectXDll.RenderPolyLine(polyLineIndex);
		}


		public void RenderTriangle(int triangleIndex)
		{
			DirectXDll.RenderTriangle(triangleIndex);
		}


		public void RenderQuad(int quadIndex)
		{
			DirectXDll.RenderQuad(quadIndex);
		}


		public void RenderEllipse(int ellipseIndex)
		{
			DirectXDll.RenderEllipse(ellipseIndex);
		}


		public void RenderBoundingBox()
		{
			DirectXDll.RenderBoundingBox();
		}


		public void RemovePolyLine(int polyLineIndex)
		{
			DirectXDll.RemovePolyLine(polyLineIndex);
		}


		public void RemoveTriangle(int triangleIndex)
		{
			DirectXDll.RemoveTriangle(triangleIndex);
		}


		public void RemoveQuad(int quadIndex)
		{
			DirectXDll.RemoveQuad(quadIndex);
		}


		public void RemoveEllipse(int ellipseIndex)
		{
			DirectXDll.RemoveEllipse(ellipseIndex);
		}
	}
}
