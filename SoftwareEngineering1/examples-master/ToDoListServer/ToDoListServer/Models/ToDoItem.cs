using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Web;

namespace ToDoListServer.Models
{
    /// <summary>
    /// A single item on a ToDo list
    /// </summary>
    [DataContract]
    public class ToDoItem
    {
        /// <summary>
        /// ID of item owner
        /// </summary>
        public string UserID { get; set; }

        /// <summary>
        /// Description of item
        /// </summary>
        public string Description { get; set; }

        /// <summary>
        /// Whether or not item has been completed
        /// </summary>
        public bool Completed { get; set; }

        /// <summary>
        /// ID number of item
        /// </summary>
        [DataMember(EmitDefaultValue = false)]
        public string ItemID { get; set; }
    }
}
