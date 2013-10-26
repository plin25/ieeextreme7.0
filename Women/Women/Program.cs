using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;

namespace Women
{
    class Program
    {
        public class Section
        {
            public uint length;
            public Section nextSection;
            public Train currentTrain;
        }

        public class Train
        {
            public enum State
            {
                Waiting,
                Accell,
                Coast,
                Decell,
                Exited,
            }

            public const double maxVelocity = 90000;
            public const double accelRate = 2700;
            public const double deccellRate = -3800;

            public Section currentSection;
            public State state;
            public double velocity;
            public double position;

            public double NextEvent(Train nextTrain)
            {
                if (state == State.Waiting)
                    return currentSection.nextSection.currentTrain.TimeTillFinishSection() + 1;
            }

            public double TimeTillFinishSection()
            {
                if (state == State.Accell)
                {
                    double timeToDecellerateFromMax = TimeTillVelocity(0, maxVelocity, deccellRate);
                    double distToDecellerate = DistanceInT(timeToDecellerateFromMax, maxVelocity, deccellRate);
                    double timeToReachMax = TimeTillVelocity(maxVelocity, velocity, accelRate);
                    double distToMax = DistanceInT(timeToReachMax, velocity, accelRate);
                    if (position + distToMax > currentSection.length - distToDecellerate)
                    {
                        var a = accelRate*(deccellRate - accelRate);
                        var b = 2*(deccellRate - accelRate)*velocity;
                        var c = -(velocity*velocity + 2*deccellRate*(currentSection.length - position));

                        var tt = (-b + Math.Sqrt(b*b - 4*a*c))/(2*a);
                        var tf = -(accelRate*tt + velocity)/deccellRate;
                        return tf + 2;
                    }
                    var coastDist = (currentSection.length - distToDecellerate) - (position + distToMax);
                    return timeToReachMax + TimeToTravelD(coastDist, maxVelocity, 0) + timeToDecellerateFromMax + 2;
                }
                if (state == State.Coast)
                {
                    double timeToDecellerateFromMax = TimeTillVelocity(0, maxVelocity, deccellRate);
                    double distToDecellerate = DistanceInT(timeToDecellerateFromMax, maxVelocity, deccellRate);
                    if (distToDecellerate > currentSection.length - position)
                        throw new InvalidOperationException();
                    double distToCoast = currentSection.length - distToDecellerate;
                    double coastTime = TimeToTravelD(distToCoast, maxVelocity, 0);
                    return timeToDecellerateFromMax + coastTime + 2;
                }
                if (state == State.Decell)
                {
                    double timeToDecellerateFromMax = TimeTillVelocity(0, maxVelocity, deccellRate);
                    return timeToDecellerateFromMax + 2;
                }
                return Double.MaxValue;
            }

            private double TimeTillVelocity(double target, double vel, double accel)
            {
                if(vel < 0) throw new InvalidOperationException();
                if (accel == 0)
                    return vel == target ? 0 : double.MaxValue;
                return (target - vel)/accel;
            }

            private static double TimeToTravelD(double dist, double vel, double accel)
            {
                if(vel < 0) throw new InvalidOperationException();
                if (dist == 0)
                    return 0;
                if (vel == 0 && accel == 0)
                    return double.MaxValue;
                return (-vel + Math.Sqrt(vel*vel - 2*accel*(-dist)))/accel;
            }

            private static double DistanceInT(double time, double vel, double accel)
            {
                if(vel < 0) throw new InvalidOperationException();
                return (accel*time*time)/2 + vel*time;
            }

            public void Advance(int time)
            {
                
            }

            public void Depart()
            {
            }

            public void Arrive()
            {
            }

            public void 
        }

        static void Main(string[] args)
        {
            try
            {
                var wholeLine = Console.ReadLine();
                var tokens = wholeLine.Split(' ');

                if(tokens.Length < 2)
                    throw new InvalidOperationException();

                var trainCount = UInt32.Parse(tokens[0]);
                if(trainCount > 5 || trainCount == 0)
                    throw new InvalidOperationException();

                var sectionLengths = tokens.Skip(1).Select(UInt32.Parse).ToList();
                if(sectionLengths.Sum(x => x) != 100000 || sectionLengths.Any(x => x < 500))
                    throw new InvalidOperationException();

            }
            catch
            {
                Console.WriteLine("ERROR");
            }
        }
    }
}
