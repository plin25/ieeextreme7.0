using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace AL
{
    class Program
    {
        internal struct Car
        {
            public char color;
            public int id;
        }

        static void Main(string[] args)
        {
            int numCars = Int32.Parse(Console.ReadLine());
            int idCounter = 0;
            var colors = Console.ReadLine().Select(x => new Car { color = x, id = idCounter++ }).ToArray();
            Console.WriteLine(countPalindromes(colors, 0, colors.Length) % 123456);
        }

        private static HashSet<string> cachedResults = new HashSet<string>();
        private static HashSet<string> palindromes = new HashSet<string>(); 

        public static BigInteger countPalindromes(Car[] cars, int left, int right)
        {
            if (left >= right)
                return 0;
            var str = String.Join(",", cars.Skip(left).Take(right - left).Select(x => x.id));
            if (cachedResults.Contains(str))
                return 0;
            cachedResults.Add(str);
            var self = getStatus(cars, left, right) ? 1 : 0;
            if (self == 1)
                palindromes.Add(str);
            var middleCut = new BigInteger(0);
            if (cars[left].color == cars[right - 1].color)
                middleCut = countPalindromes(cars, left + 1, right - 1) + self;
            var beginCut = countPalindromes(cars, left + 1, right);
            var endCut = countPalindromes(cars, left, right - 1);
            return middleCut + beginCut + endCut;
        }

        public static bool getStatus(Car[] cars, int left, int right)
        {
            if (left == right - 1) return true;
            if (left >= right) return false;
            if (left == right - 2)
                return cars[left].color == cars[right - 1].color;
            for (int i = left; i < left + Math.Floor((right - left) / 2.0); i++)
            {
                if (cars[i].color != cars[right - i - 1].color)
                    return false;
            }
            return true;
        }
    }
}
