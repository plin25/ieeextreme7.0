using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace AR
{
    class Program
    {
        private Dictionary<int, List<int>> closestSums; 
        static void Main(string[] args)
        {
            string line = "";
            int N = 0, K = 0;

            List<ulong> answers = new List<ulong>();

            do
            {
                line = Console.ReadLine();
                var splits = line.Split(' ');
                N = Int32.Parse(splits[0]);
                K = Int32.Parse(splits[1]);
                if (N == 0 && K == 0)
                    break;
                answers.Add(2 * Downs(N,N,K));
            } while (N != 0 && K != 0);
            Console.WriteLine(String.Join("\n", answers));

        }

        private static Dictionary<Tuple<int, int, int>, ulong> DownResults = new Dictionary<Tuple<int, int, int>, ulong>();
        private static Dictionary<Tuple<int, int, int>, ulong> RightResults = new Dictionary<Tuple<int, int, int>, ulong>(); 

        public static ulong Downs(int x, int y, int k)
        {
            if (y > 1 && x == 1 && k == 0)
                return 1;
            if (y == 1 || k == 0)
                return 0;
            var key = Tuple.Create(x, y, k);
            if (DownResults.ContainsKey(key))
                return DownResults[key];
            var result = Rights(x, y - 1, k - 1) + Downs(x, y - 1, k);
            DownResults[key] = result;
            return result;
        }

        public static ulong Rights(int x, int y, int k)
        {
            if (x > 1 && y == 1 && k == 0)
                return 1;
            if (x == 1 || k == 0)
                return 0;
            var key = Tuple.Create(x, y, k);
            if (RightResults.ContainsKey(key))
                return RightResults[key];
            var result = Downs(x - 1, y, k - 1) + Rights(x - 1, y, k);
            RightResults[key] = result;
            return result;
        }
    }
}
