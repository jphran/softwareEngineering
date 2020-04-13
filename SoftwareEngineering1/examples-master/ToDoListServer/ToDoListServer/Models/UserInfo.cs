using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace ToDoListServer.Models
{
    /// <summary>
    /// Information about a ToDo list user
    /// </summary>
    public class UserInfo
    {
        /// <summary>
        /// Name of user
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// Email address of user
        /// </summary>
        public string Email { get; set; }
    }
}