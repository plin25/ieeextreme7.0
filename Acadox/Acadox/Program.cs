using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace Acadox
{
    class Program
    {
        abstract class Token
        {
            public abstract uint Evaluate(Stack<Token> stack);

            public static Token Parse(string t)
            {
                switch (t.ToUpper())
                {
                    case "+":
                        return new Addition();
                    case "-":
                        return new Subtraction();
                    case "&":
                        return new LogicalAnd();
                    case "|":
                        return new LogicalOr();
                    case "~":
                        return new LogicalNot();
                    case "X":
                        return new LogicalXor();
                    default:
                        uint value = 0;
                        if (t.StartsWith("0x"))
                            t = t.Replace("0x", "");
                        if (t.StartsWith("0X"))
                            t = t.Replace("0X", "");
                        if (t.Length > 4)
                            return new Invalid();
                        if (UInt32.TryParse(t, NumberStyles.HexNumber, null, out value))
                        {
                            return new Digit(value);
                        }
                        return new Invalid();
                }
            }
        }

        class Digit : Token
        {
            private readonly uint value;

            public Digit(uint value)
            {
                this.value = Math.Max(0, Math.Min(0xFFFF, value));
            }

            public override uint Evaluate(Stack<Token> stack)
            {
                return value;
            }
        }

        class Addition : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                try
                {
                    uint first = stack.Pop().Evaluate(stack);
                    uint second = stack.Pop().Evaluate(stack);
                    return first + second;
                }
                catch
                {
                    stack.Push(new Invalid());
                    return 0;
                }
            }
        }

        class Subtraction : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                try
                {
                    uint first = stack.Pop().Evaluate(stack);
                    uint second = stack.Pop().Evaluate(stack);
                    if (first >= second)
                        return 0;
                    return second - first;
                }
                catch
                {
                    stack.Push(new Invalid());
                    return 0;
                }
            }
        }

        class LogicalAnd : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                try
                {
                    uint first = stack.Pop().Evaluate(stack);
                    uint second = stack.Pop().Evaluate(stack);
                    return first & second;
                }
                catch
                {
                    stack.Push(new Invalid());
                    return 0;
                }
            }
        }

        class LogicalOr : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                try
                {
                    uint first = stack.Pop().Evaluate(stack);
                    uint second = stack.Pop().Evaluate(stack);
                    return first | second;
                }
                catch
                {
                    stack.Push(new Invalid());
                    return 0;
                }
            }
        }

        class LogicalNot : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                try
                {
                    uint first = stack.Pop().Evaluate(stack);
                    return 0xFFFF - first;
                }
                catch
                {
                    stack.Push(new Invalid());
                    return 0;
                }
            }
        }

        class LogicalXor : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                try
                {
                    uint first = stack.Pop().Evaluate(stack);
                    uint second = stack.Pop().Evaluate(stack);
                    return first ^ second;
                }
                catch
                {
                    stack.Push(new Invalid());
                    return 0;
                }
            }
        }
        class Invalid : Token
        {

            public override uint Evaluate(Stack<Token> stack)
            {
                stack.Push(this);
                return 0;
            }
        }

        static void Main(string[] args)
        {
            var fullString = Console.ReadLine();
            if (String.IsNullOrWhiteSpace(fullString))
            {
                Console.WriteLine("ERROR");
                return;
            }
            char[] whitespace = {' ', '\t'};
            string[] tokens = fullString.Split(whitespace);

            if (tokens.Length > 20)
            {
                Console.WriteLine("ERROR");
                return;
            }

            var stack = new Stack<Token>();

            foreach (var tok in tokens)
            {
                if (stack.Any(t => t is Invalid))
                {
                    Console.WriteLine("ERROR");
                    return;
                }
                stack.Push(new Digit(Token.Parse(tok).Evaluate(stack)));
            }

            if (stack.Count == 1 && stack.Peek() is Digit)
            {
                Console.WriteLine("{0:X4}", stack.Peek().Evaluate(stack));
            }
            else
            {
                Console.WriteLine("ERROR");
            }
        }
    }
}
