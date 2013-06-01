using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace img_detective.ui.model
{
    public class SearchResult
    {
        #region nested

        public struct Node
        {
            public long ImageId { get; set; }
            public double Position { get; set; }
        }

        #endregion

        #region properties

        public List<Node> Nodes { get; private set; }

        #endregion

        #region ctor

        public SearchResult() 
        {
            Nodes = new List<Node>();
        }

        public SearchResult(long[] imageIds, double[] positions)
            : this()
        {
            Contract.Requires(imageIds != null);
            Contract.Requires(positions != null);
            Contract.Requires(imageIds.Length == positions.Length);

            Nodes.Capacity = imageIds.Length;

            for (int i = 0; i < imageIds.Length; i++)
            {
                Nodes.Add(new Node()
                {
                    ImageId = imageIds[i],
                    Position = positions[i]
                });
            }
        }

        #endregion

        
    }
}
