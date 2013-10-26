using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Resources;
using System.Runtime.Remoting.Messaging;
using System.Runtime.Remoting.Metadata.W3cXsd2001;
using System.Security.Cryptography.X509Certificates;
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
            public Section prevSection;
            public Train currentTrain;
            public bool open = false;
        }

        public class ScheduleEntry
        {
            public double departTime;
            public double arriveTime;
        }

        public class Train
        {
            public List<ScheduleEntry> schedule = new List<ScheduleEntry>(); 

            public enum State
            {
                New,
                JustArrived,
                Waiting,
                Ready,
                Leaving,
                Accelerating,
                Coasting,
                Deccelerating,
                Depot,
                Done,
            }

            public const double maxVelocity = 25;
            public const double accelRate = 0.75;
            public const double deccellRate = -1.05555556;
            public const double TimeToLeaveStation = accelRate;

            public const double TIME_IN_STATION = 120;
            public const double BUFFER_TIME = 1;
            public const double TRAIN_LENGTH = 100;

            public Section currentSection;
            public Section oldSection;
            public State state;
            public double timeLeftInState = 0;
            public double positionInSection = 0;
            public double velocity = 0;
            private bool blocked = false;
            private double totalTime = 0;
            public bool needsCorrection = false;
            public double TimeTillTransition()
            {
                switch (state)
                {
                    case State.New:
                        return timeLeftInState;
                    case State.Done:
                        return Double.MaxValue;
                    case State.Waiting:
                        if (currentSection.nextSection == null)
                            return 0;
                        if (!currentSection.nextSection.open)
                        {
                            blocked = true;
                            return Double.MaxValue;
                        }
                        return 0;
                    case State.JustArrived:
                        return timeLeftInState;
                    case State.Ready:
                        return timeLeftInState;
                    case State.Leaving:
                        return TimeToTravel(TRAIN_LENGTH - positionInSection, velocity, accelRate);
                    case State.Accelerating:
                    case State.Coasting:
                        return TimeToTurnover(velocity, currentSection.length - positionInSection);
                    case State.Deccelerating:
                        return TimeToTravel(currentSection.length - positionInSection, velocity, deccellRate);
                    case State.Depot:
                        return timeLeftInState;
                    default:
                        throw new InvalidOperationException();
                }
            }
            const double TOLERANCE = 0.000000001;

            public void Advance(double time)
            {
                if(time > TimeTillTransition())
                    throw new InvalidOperationException();
                totalTime += time;

                switch (state)
                {
                    case State.New:
                        timeLeftInState -= time;
                        if (Math.Abs(timeLeftInState) < TOLERANCE)
                        {
                            RoundingCorrect(Math.Max(0, Math.Round(totalTime) - totalTime));
                            schedule.Add(new ScheduleEntry() { departTime = totalTime, arriveTime = -1 });
                            state = State.Leaving;
                        }
                        break;
                    case State.Done:
                        return;
                    case State.Waiting:
                        if (currentSection.nextSection == null)
                            throw new InvalidOperationException();
                        if (currentSection.nextSection.open)
                        {
                            RoundingCorrect(Math.Max(0, Math.Round(totalTime + 1) - totalTime));
                            schedule.Add(new ScheduleEntry() { departTime = totalTime, arriveTime = -1 });
                            state = State.Ready;
                            oldSection = currentSection;
                            currentSection = currentSection.nextSection;
                            currentSection.currentTrain = this;
                            oldSection.currentTrain = null;
                            currentSection.open = false;
                            positionInSection = 0;
                            timeLeftInState = BUFFER_TIME;
                        }
                        break;
                    case State.JustArrived:
                        timeLeftInState -= time;
                        if(Math.Abs(timeLeftInState) < TOLERANCE)
                            state = State.Waiting;
                        break;
                    case State.Ready:
                        timeLeftInState -= time;
                        if(Math.Abs(timeLeftInState) < TOLERANCE)
                            state = State.Leaving;
                        break;
                    case State.Leaving:
                        Integrate(time, accelRate);
                        if (Math.Abs(positionInSection - TRAIN_LENGTH) < TOLERANCE)
                        {
                            state = State.Accelerating;
                            schedule.Last().departTime = totalTime;
                            if(oldSection != null)
                                oldSection.open = true;
                        }
                        break;
                    case State.Accelerating:
                        Integrate(time, accelRate);
                        if (Math.Abs(TimeToTurnover(velocity, currentSection.length - positionInSection)) < TOLERANCE ||
                            Math.Abs(velocity - maxVelocity) < TOLERANCE)
                            state = State.Coasting;
                        break;
                    case State.Coasting:
                        Integrate(time, 0);
                        if (Math.Abs(TimeToTurnover(velocity, currentSection.length - positionInSection)) < TOLERANCE)
                            state = State.Deccelerating;
                        break;
                    case State.Deccelerating:
                        Integrate(time, deccellRate);
                        if (Math.Abs(velocity) < TOLERANCE)
                        {
                            RoundingCorrect(Math.Max(0, Math.Floor(totalTime) - totalTime));
                            schedule.Last().arriveTime = totalTime;
                            if (currentSection.nextSection == null)
                            {
                                state = State.Depot;
                                timeLeftInState = 1;
                            }
                            else
                            {
                                state = State.JustArrived;
                                timeLeftInState = TIME_IN_STATION;
                            }
                        }
                        break;
                    case State.Depot:
                        timeLeftInState -= time;
                        if(Math.Abs(timeLeftInState) < TOLERANCE)
                            state = State.Done;
                        break;
                    default:
                        throw new InvalidOperationException();                    
                }
            }

            public void RoundingCorrect(double time)
            {
                totalTime += time;
            }

            private void Integrate(double time, double accel)
            {
                positionInSection += accel * time * time / 2 + velocity * time;
                velocity += accel * time;
            }

            public static double TimeToTurnover(double velocity, double distanceLeft)
            {
                var timeToMax = TimeToVelocity(velocity, maxVelocity, accelRate);
                var timeFromMax = TimeToVelocity(maxVelocity, 0, deccellRate);

                var distanceToMax = accelRate * timeToMax * timeToMax / 2 + velocity * timeToMax;
                var distanceFromMax = deccellRate * timeFromMax * timeFromMax / 2 + maxVelocity * timeFromMax;

                if (Math.Abs(velocity - maxVelocity) < TOLERANCE)
                {
                    return TimeToTravel(distanceLeft - distanceFromMax, velocity, 0);
                }

                if (distanceToMax + distanceFromMax > distanceLeft)
                {
                    var a = accelRate * (deccellRate - accelRate);
                    var b = 2 * (deccellRate - accelRate) * velocity;
                    var c = -(velocity * velocity + 2 * deccellRate * distanceLeft);

                    var tt1 = (-b + Math.Sqrt(b * b - 4 * a * c)) / (2 * a);
                    var tt2 = (-b - Math.Sqrt(b * b - 4 * a * c)) / (2 * a);
                    return Math.Max(tt1, tt2);                    
                }
                else
                {
                    return timeToMax;
                }
            }

            public static double TimeToVelocity(double velocity, double target, double acceleration)
            {
                return (target - velocity)/acceleration;
            }

            public static double TimeToTravel(double dist, double vel, double accel)
            {
                if (vel < 0) throw new InvalidOperationException();
                if (Math.Abs(dist) < TOLERANCE)
                    return 0;
                if (Math.Abs(vel) < TOLERANCE && Math.Abs(accel) < TOLERANCE)
                    return double.MaxValue;
                if (Math.Abs(accel) < TOLERANCE)
                    return dist/vel;
                if (Math.Abs(vel*vel - 2*accel*(-dist)) < TOLERANCE)
                    return -vel/accel;
                return (-vel + Math.Sqrt(vel * vel - 2 * accel * (-dist))) / accel;

            }


            //public double NextEvent(Train nextTrain)
            //{
            //    if (state == State.Waiting)
            //        return currentSection.nextSection.currentTrain.TimeTillFinishSection() + 1;
            //}

            //public double TimeTillFinishSection()
            //{
            //    if (state == State.Accell)
            //    {
            //        double timeToDecellerateFromMax = TimeTillVelocity(0, maxVelocity, deccellRate);
            //        double distToDecellerate = DistanceInT(timeToDecellerateFromMax, maxVelocity, deccellRate);
            //        double timeToReachMax = TimeTillVelocity(maxVelocity, velocity, accelRate);
            //        double distToMax = DistanceInT(timeToReachMax, velocity, accelRate);
            //        if (position + distToMax > currentSection.length - distToDecellerate)
            //        {
            //            var a = accelRate*(deccellRate - accelRate);
            //            var b = 2*(deccellRate - accelRate)*velocity;
            //            var c = -(velocity*velocity + 2*deccellRate*(currentSection.length - position));

            //            var tt = (-b + Math.Sqrt(b*b - 4*a*c))/(2*a);
            //            var tf = -(accelRate*tt + velocity)/deccellRate;
            //            return tf + 2;
            //        }
            //        var coastDist = (currentSection.length - distToDecellerate) - (position + distToMax);
            //        return timeToReachMax + TimeToTravelD(coastDist, maxVelocity, 0) + timeToDecellerateFromMax + 2;
            //    }
            //    if (state == State.Coast)
            //    {
            //        double timeToDecellerateFromMax = TimeTillVelocity(0, maxVelocity, deccellRate);
            //        double distToDecellerate = DistanceInT(timeToDecellerateFromMax, maxVelocity, deccellRate);
            //        if (distToDecellerate > currentSection.length - position)
            //            throw new InvalidOperationException();
            //        double distToCoast = currentSection.length - distToDecellerate;
            //        double coastTime = TimeToTravelD(distToCoast, maxVelocity, 0);
            //        return timeToDecellerateFromMax + coastTime + 2;
            //    }
            //    if (state == State.Decell)
            //    {
            //        double timeToDecellerateFromMax = TimeTillVelocity(0, maxVelocity, deccellRate);
            //        return timeToDecellerateFromMax + 2;
            //    }
            //    return Double.MaxValue;
            //}

            //private double TimeTillVelocity(double target, double vel, double accel)
            //{
            //    if(vel < 0) throw new InvalidOperationException();
            //    if (accel == 0)
            //        return vel == target ? 0 : double.MaxValue;
            //    return (target - vel)/accel;
            //}

            //private static double TimeToTravelD(double dist, double vel, double accel)
            //{
            //    if(vel < 0) throw new InvalidOperationException();
            //    if (dist == 0)
            //        return 0;
            //    if (vel == 0 && accel == 0)
            //        return double.MaxValue;
            //    return (-vel + Math.Sqrt(vel*vel - 2*accel*(-dist)))/accel;
            //}

            //private static double DistanceInT(double time, double vel, double accel)
            //{
            //    if(vel < 0) throw new InvalidOperationException();
            //    return (accel*time*time)/2 + vel*time;
            //}

            //public void Advance(int time)
            //{
                
            //}

            //public void Depart()
            //{
            //}

            //public void Arrive()
            //{
            //}

            //public void 
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

                List<Section> sections = new List<Section>();
                foreach (var length in sectionLengths)
                {
                    Section last = null;
                    if (sections.Count >= 1)
                        last = sections.Last();
                    var section = new Section {length = length, prevSection = last, open = true };
                    sections.Add(section);
                    if(last != null)
                        last.nextSection = section;
                }

                List<Train> trains = new List<Train>();
                Stack<Train> undepartedTrains = new Stack<Train>();
                for (int x = 0; x < trainCount; x++)
                {
                    var t = new Train {currentSection = null, state = Train.State.Waiting};
                    trains.Add(t);
                    undepartedTrains.Push(t);
                }
                double totalTime = 0;
                while (trainCount > 0)
                {
                    if (sections.First().open && undepartedTrains.Count >= 1)
                    {
                        // Introduce a new train
                        var t = undepartedTrains.Pop();
                        sections.First().currentTrain = t;
                        t.currentSection = sections.First();
                        t.currentSection.open = false;
                            t.state = Train.State.New;
                            t.timeLeftInState = 1;
                    }

                    var minTime =
                        sections.Select(x => x.currentTrain).Where(x => x != null).Min(x => x.TimeTillTransition());
                    if(minTime < 0)
                        throw new InvalidOperationException();
                    sections.ForEach(y =>
                    {
                        if (y.currentTrain != null)
                            y.currentTrain.Advance(minTime);
                    });
                    totalTime += minTime;
                    if (sections.Last().currentTrain != null && sections.Last().currentTrain.state == Train.State.Done)
                    {
                        sections.Last().currentTrain = null;
                        sections.Last().open = true;
                        trainCount--;
                    }
                }
                foreach (var train in trains.Reverse<Train>().Select((t, idx) => new {t, idx}))
                {
                    Console.WriteLine(
                        String.Format("{0} : ***** - {1} *****",
                        train.idx + 1,
                        String.Join(" - ", train.t.schedule.Select(se => String.Format("{0,5:F0}  {1,5:}", se.departTime, se.arriveTime)))));
                }
                //Console.WriteLine("1 : ***** -     1   {0} *****	", totalTime);
            }
            catch
            {
                Console.WriteLine("ERROR");
            }
        }
    }
}
