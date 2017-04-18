using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KustomPhotoshop.Interfaces
{
	public interface IDrawable
	{
		void Draw(IRenderer renderer);
	}
}
