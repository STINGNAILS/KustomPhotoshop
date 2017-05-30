using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Runtime.InteropServices;


namespace KustomPhotoshop
{
	public partial class DirectXControl : UserControl
	{
		public bool isInitialized = false;


		public DirectXControl()
		{
			InitializeComponent();
		}


		public void Init()
		{
			DirectXDll.Init((int)Handle);
			isInitialized = true;
		}


		public void Release()
		{
			DirectXDll.Release();
		}


		public void BeginRender()
		{
			if(isInitialized)
			{
				DirectXDll.BeginRender();
			}
		}


		public void FinishRender()
		{
			if(isInitialized)
			{
				DirectXDll.FinishRender();
			}
		}


		public void SetCameraPosition(Point position)
		{
			DirectXDll.SetCameraPosition((float)position.X, (float)position.Y);
		}


		public void SetCameraZoom(float scale)
		{
			DirectXDll.SetCameraZoom(scale);
		}


		public void SetBoundingBoxZoom(float scale)
		{
			DirectXDll.SetBoundingBoxZoom(scale);
		}


		protected override void OnPaintBackground(PaintEventArgs pevent)
		{

		}


		public void DirectXControl_SizeChanged(object sender, EventArgs e)
		{
			if(isInitialized)
			{
				DirectXDll.Resize((int)Handle);
			}
			Invalidate();
		}
	}
}


public class DirectXDll
{
	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Init")]
	public static extern void Init(int hwnd);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Resize")]
	public static extern void Resize(int hwnd);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "Release")]
	public static extern void Release();

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "BeginRender")]
	public static extern void BeginRender();

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "FinishRender")]
	public static extern void FinishRender();

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetCameraPosition")]
	public static extern void SetCameraPosition(float x, float y);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetCameraZoom")]
	public static extern void SetCameraZoom(float scale);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "SetBoundingBoxZoom")]
	public static extern void SetBoundingBoxZoom(float scale);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "AddPolyLine")]
	public static extern int AddPolyLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int pointsNum, float r, float g, float b, float width);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "AddTriangle")]
	public static extern int AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float rB, float gB, float bB, float width);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ModifyPolyLine")]
	public static extern void ModifyPolyLine(int polyLineIndex, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int pointsNum, float r, float g, float b, float width);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ModifyTriangle")]
	public static extern void ModifyTriangle(int triangleIndex, float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b, float rB, float gB, float bB, float width);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "ModifyBoundingBox")]
	public static extern void ModifyBoundingBox(float xMin, float yMin, float xMax, float yMax);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RenderPolyLine")]
	public static extern void RenderPolyLine(int polyLineIndex);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RenderTriangle")]
	public static extern void RenderTriangle(int triangleIndex);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RenderBoundingBox")]
	public static extern void RenderBoundingBox();

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RemovePolyLine")]
	public static extern void RemovePolyLine(int polyLineIndex);

	[DllImport("DirectXRenderer.dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RemoveTriangle")]
	public static extern void RemoveTriangle(int triangleIndex);
}