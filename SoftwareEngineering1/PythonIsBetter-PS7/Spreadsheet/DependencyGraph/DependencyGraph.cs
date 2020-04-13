// Skeleton implementation written by Joe Zachary for CS 3500, January 2018.
// Added to by Adam Gaia u0344827

using System;
using System.Collections.Generic;

namespace Dependencies
{
    /// <summary>
    /// A DependencyGraph can be modeled as a set of dependencies, where a dependency is an ordered 
    /// pair of strings.  Two dependencies (s1,t1) and (s2,t2) are considered equal if and only if 
    /// s1 equals s2 and t1 equals t2.
    /// 
    /// Given a DependencyGraph DG:
    /// 
    ///    (1) If s is a string, the set of all strings t such that the dependency (s,t) is in DG 
    ///    is called the dependents of s, which we will denote as dependents(s).
    ///        
    ///    (2) If t is a string, the set of all strings s such that the dependency (s,t) is in DG 
    ///    is called the dependees of t, which we will denote as dependees(t).
    ///    
    /// The notations dependents(s) and dependees(s) are used in the specification of the methods of this class.
    ///
    /// For example, suppose DG = {("a", "b"), ("a", "c"), ("b", "d"), ("d", "d")}
    ///     dependents("a") = {"b", "c"}
    ///     dependents("b") = {"d"}
    ///     dependents("c") = {}
    ///     dependents("d") = {"d"}
    ///     dependees("a") = {}
    ///     dependees("b") = {"a"}
    ///     dependees("c") = {"a"}
    ///     dependees("d") = {"b", "d"}
    ///     
    /// All of the methods below throw a ArgumentNullException if a param is null.
    ///
    /// IMPORTANT IMPLEMENTATION NOTE
    /// 
    /// The simplest way to describe a DependencyGraph and its methods is as a set of dependencies, 
    /// as discussed above.
    /// 
    /// However, physically representing a DependencyGraph as, say, a set of ordered pairs will not
    /// yield an acceptably efficient representation.  DO NOT USE SUCH A REPRESENTATION.
    /// 
    /// You'll need to be more clever than that.  Design a representation that is both easy to work
    /// with as well acceptably efficient according to the guidelines in the PS3 writeup. Some of
    /// the test cases with which you will be graded will create massive DependencyGraphs.  If you
    /// build an inefficient DependencyGraph this week, you will be regretting it for the next month.
    /// </summary>
    public class DependencyGraph
    {
        private int numDependencies;

        // Dictionary representation of the DependencyGraph.
        private Dictionary<string, Node> dependGraph;

        /// <summary>
        /// Nodes are used to keep track of dependents and dependees for each s or t
        /// </summary>
        public class Node
        {
            private HashSet<string> dependents;
            private int numDependents;
            private HashSet<string> dependees;
            private int numDependees;

            /// <summary>
            /// Helper method to directly import IEnumerables to hashsets
            /// </summary>
            public void importData(IEnumerable<string> ees, IEnumerable<string> ents)
            {
                this.dependees.UnionWith(ees);
                this.numDependees = this.dependees.Count;
                
                this.dependents.UnionWith(ents);
                this.numDependents = this.dependents.Count;
            }

            public Node()
            {
                this.dependents = new HashSet<string>();
                this.numDependents = 0;
                this.dependees = new HashSet<string>();
                this.numDependees = 0;
            }

            public void addDependent(string dependent)
            {
                this.dependents.Add(dependent);
                numDependents++;
            }

            public void addDependee(string dependee)
            {
                this.dependees.Add(dependee);
                numDependees++;
            }

            public int getNumDependents()
            {
                return this.numDependents;
            }

            public int getNumDependees()
            {
                return this.numDependees;
            }

            public IEnumerable<string> getListOfDependents()
            {
                return this.dependents;
            }
            public IEnumerable<string> getListOfDependees()
            {
                return this.dependees;
            }

            public bool containsDependant(string t)
            {
                if (this.dependents.Contains(t))
                {
                    return true;
                }
                return false;
            }

            public bool containsDependee(string s)
            {
                if (this.dependees.Contains(s))
                {
                    return true;
                }
                return false;
            }

            public void removeDependant(string t)
            {
                this.dependents.Remove(t);
                this.numDependents--;
            }

            public void removeDependee(string s)
            {
                this.dependees.Remove(s);
                this.numDependees--;
            }

        } // end node class

        /// <summary>
        /// Creates a DependencyGraph containing no dependencies.
        /// </summary>
        public DependencyGraph()
        {
            this.dependGraph = new Dictionary<string, Node>();
            this.numDependencies = 0;
        }

        /// <summary>
        /// Create a new DG from an existing DG
        /// </summary>
        /// <param name="oldDG"></param>
        public DependencyGraph(DependencyGraph oldDG)
        {
            if (oldDG == null)
            {
                throw new ArgumentNullException("Null input param");
            }

            this.dependGraph = new Dictionary<string, Node>();
            int oldSize = oldDG.dependGraph.Count;

            List<string> keyList = new List<string>(oldSize);
            keyList.AddRange(oldDG.dependGraph.Keys);

            List<Node> nodeList = new List<Node>(oldSize);
            nodeList.AddRange(oldDG.dependGraph.Values);

            Console.WriteLine("test5");
            int idx = 0;
            foreach (Node n in nodeList)
            {
                IEnumerable<string> ees = n.getListOfDependees();
                IEnumerable<string> ents = n.getListOfDependents();

                Console.WriteLine("test6");
                Node newNode = new Node();
                newNode.importData(ees, ents);

                Console.WriteLine("test7");
                this.dependGraph.Add(keyList[idx], newNode);
                Console.WriteLine("test8");
                idx++;
            }
        }


        /// <summary>
        /// The number of dependencies in the DependencyGraph.
        /// </summary>
        public int Size
        {
            get { return this.numDependencies; }
        }

        /// <summary>
        /// Reports whether dependents(s) is non-empty.  
        /// </summary>
        public bool HasDependents(string s)
        {
            // Check for null input
            if (s == null)
            {
                throw new ArgumentNullException("Null input");
            }

            // Find node specified by input argument s
            Node n;
            if(!this.dependGraph.TryGetValue(s, out n))
            {
                // return false if trygetvalue fails
                return false;
            }

            // Check num dependents
            if (n.getNumDependents() == 0)
            {
                return false;
            }
            // else
            return true;
        }

        /// <summary>
        /// Reports whether dependees(s) is non-empty.  
        /// </summary>
        public bool HasDependees(string s)
        {

            // Check for null input
            if (s == null)
            {
                throw new ArgumentNullException("Null input");
            }

            // Find node specified by input argument s
            Node n;
            if (!this.dependGraph.TryGetValue(s, out n))
            {
                // return false if trygetvalue fails
                return false;
            }

            // Check num dependees
            if (n.getNumDependees() == 0)
            {
                return false;
            }
            // else
            return true;
        }

        /// <summary>
        /// Enumerates dependents(s).  
        /// </summary>
        public IEnumerable<string> GetDependents(string s)
        {

            // Check for null input
            if (s == null)
            {
                throw new ArgumentNullException("Null input");
            }

            Node n;
            if(this.dependGraph.TryGetValue(s, out n))
            {
                return n.getListOfDependents();
            }
            // else, if no node s exists, return empty list
            return new List<string>();
        }

        /// <summary>
        /// Enumerates dependees(s).  
        /// </summary>
        public IEnumerable<string> GetDependees(string s)
        {

            // Check for null input
            if (s == null)
            {
                throw new ArgumentNullException("Null input");
            }


            Node n;
            if(this.dependGraph.TryGetValue(s, out n))
            {
                return n.getListOfDependees();
            }
            // else, if no node s exists, return empty list
            return new List<string>();
        }

        /// <summary>
        /// Adds the dependency (s,t) to this DependencyGraph.
        /// This has no effect if (s,t) already belongs to this DependencyGraph.
        /// </summary>
        public void AddDependency(string s, string t)
        {
            // Check for null input
            if (s == null || t == null)
            {
                throw new ArgumentNullException("Null input");
            }



            // Check if s already exists
            Node sNode;
            if (this.dependGraph.TryGetValue(s, out sNode))
            {
                // Check if t is already a dependant of s
                if (sNode.containsDependant(t))
                {
                    // no need to update
                    return;
                }
                // else
                sNode.addDependent(t);
            }
            else
            {
                sNode = new Node();
                sNode.addDependent(t);
                // add sNode to dependGraph
                this.dependGraph.Add(s, sNode);
            }

            // Now take care of t
            // Check if a node for t exists
            Node tNode;
            if (this.dependGraph.TryGetValue(t, out tNode))
            {
                tNode.addDependee(s);
            }
            else
            {
                tNode = new Node();
                tNode.addDependee(s);
                // add tNode
                this.dependGraph.Add(t, tNode);
            }

            // update number of dependencies
            this.numDependencies++;

        }


        // Now create node for 

        /// <summary>
        /// Removes the dependency (s,t) from this DependencyGraph.
        /// Does nothing if (s,t) doesn't belong to this DependencyGraph.
        /// </summary>
        public void RemoveDependency(string s, string t)
        {
            // Check for null input
            if (s == null || t == null)
            {
                throw new ArgumentNullException("Null input");
            }


            // Check if s already exists
            Node sNode;
            if (this.dependGraph.TryGetValue(s, out sNode))
            {
                // Check if t is a dependant of s
                if (sNode.containsDependant(t))
                {
                    // Remove t from s dependant list
                    sNode.removeDependant(t);

                    // Now take care of t
                    // Check if a node for t exists
                    Node tNode;
                    if (this.dependGraph.TryGetValue(t, out tNode))
                    {
                        // Check that s is a dependee of t (should always be, but just incase better check)
                        if (tNode.containsDependee(s))
                        {
                            tNode.removeDependee(s);
                        }
            
                    }

                    // update number of dependencies
                    this.numDependencies--;
                }
            }
        }

        /// <summary>
        /// Removes all existing dependencies of the form (s,r).  Then, for each
        /// t in newDependents, adds the dependency (s,t).
        /// 
        /// If new dependency is null, thows ArgumentNullException
        /// 
        /// </summary>
        public void ReplaceDependents(string s, IEnumerable<string> newDependents)
        {

            // Check for null input
            if (s == null || newDependents == null)
            {
                throw new ArgumentNullException("Null input");
            }




            Node sNode;
            if(this.dependGraph.TryGetValue(s, out sNode))
            {
                // Loop through current dependencies of s, removing each one
                List<string> dependentsToChange = new List<string>();
                dependentsToChange.AddRange(sNode.getListOfDependents());
                foreach (string r in dependentsToChange)
                {
                    this.RemoveDependency(s, r);
                }

                // add all of the new dependencies
                foreach (string t in newDependents)
                {
                    // Check for null input
                    if (t == null)
                    {
                        throw new ArgumentNullException("Null input");
                    }


                    this.AddDependency(s, t);
                }
            }
            // else, no node s exists, do nothing
            
        }

        /// <summary>
        /// Removes all existing dependencies of the form (r,t).  Then, for each 
        /// s in newDependees, adds the dependency (s,t).
        /// 
        /// If new dependency is null, thows ArgumentNullException
        /// 
        /// </summary>
        public void ReplaceDependees(string t, IEnumerable<string> newDependees)
        {


            // Check for null input
            if (t == null || newDependees == null)
            {
                throw new ArgumentNullException("Null input");
            }


            Node tNode;
            if(this.dependGraph.TryGetValue(t, out tNode))
            {
                // Loop through current dependee of t, removing each one
                List<string> dependeesToChange = new List<string>();
                dependeesToChange.AddRange(tNode.getListOfDependees());
                foreach (string r in dependeesToChange)
                {
                    this.RemoveDependency(r, t);
                }

                // add all of the new dependencies
                foreach (string s in newDependees)
                {

                    if (s == null)
                    {
                        throw new ArgumentNullException("Null input");
                    }



                    this.AddDependency(s, t);
                }
            }
            // else, no node t exists, do nothing

        }


    } // end DG class
}
