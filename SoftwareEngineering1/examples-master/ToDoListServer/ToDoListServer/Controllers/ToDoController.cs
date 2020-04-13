using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading;
using System.Web.Http;
using ToDoListServer.Models;

namespace ToDoListServer.Controllers
{
    /// <summary>
    /// Defines and implements the service interface for the server
    /// </summary>
    public class ToDoController : ApiController
    {
        /// <summary>
        /// Registers a new user.
        /// If either user.Name or user.Email is null or is empty after trimming, responds with status code Forbidden.
        /// Otherwise, creates a user, returns the user's token, and responds with status code Ok. 
        /// </summary>
        /// <param name="user">User to be added to ToDo list</param>
        /// <returns>ID number of newly added user</returns>
        [Route("ToDo/RegisterUser")]
        public string PostRegister(UserInfo user)
        {
            if (user.Name == "stall")
            {
                Thread.Sleep(5000);
            }
            lock (sync)
            {
                if (user.Name == null || user.Name.Trim().Length == 0)
                {
                    throw new HttpResponseException(HttpStatusCode.Forbidden);
                }
                else
                {
                    string userID = Guid.NewGuid().ToString();
                    users.Add(userID, user);
                    return userID;
                }
            }
        }

        /// <summary>
        /// Adds an item to the ToDo list.  
        /// If item.UserID isn't known, responds with status code Forbidden.
        /// Othewise, adds the item to the list, returns the new ItemID, and responds with status code Ok.
        /// </summary>
        /// <param name="item">Item to be added to ToDo list</param>
        /// <returns>ID number of newly added item</returns>
        [Route("ToDo/AddItem")]
        public string PostAddItem(Item item)
        {
            lock (sync)
            {
                if (item.UserID == null || !users.ContainsKey(item.UserID))
                {
                    throw new HttpResponseException(HttpStatusCode.Forbidden);
                }
                else
                {
                    string itemID = Guid.NewGuid().ToString();
                    ToDoItem toDoItem = new ToDoItem()
                    {
                        UserID = item.UserID,
                        Description = item.Description,
                        ItemID = itemID,
                        Completed = false
                    };
                    items.Add(itemID, toDoItem);
                    return itemID;
                }
            }
        }

        /// <summary>
        /// Marks an item as completed.
        /// If itemID is unknown, responds with status code Forbidden.
        /// Otherwise, marks the item as completeed and responds with status code OK.
        /// </summary>
        /// <param name="itemID">ID of item to be marked completed</param>
        [Route("ToDo/MarkCompleted/{itemID}")]
        public void PutMarkCompleted(string itemID)
        {
            lock (sync)
            {
                ToDoItem item;
                if (!items.TryGetValue(itemID, out item))
                {
                    throw new HttpResponseException(HttpStatusCode.Forbidden);
                }
                else
                {
                    item.Completed = true;
                }
            }
        }

        /// <summary>
        /// Deletes an item.
        /// If itemID is unknown, responds with status code Forbidden.
        /// Otherwise, deletes the item and responds with status code OK.
        /// </summary>
        /// <param name="itemID">ID of item to be deleted</param>
        [Route("ToDo/DeleteItem/{itemID}")]
        public void DeleteItem(string itemID)
        {
            lock (sync)
            {
                if (!items.ContainsKey(itemID))
                {
                    throw new HttpResponseException(HttpStatusCode.Forbidden);
                }
                else
                {
                    items.Remove(itemID);
                }
            }
        }

        /// <summary>
        /// Returns a list of ToDoItems.  
        /// If userID is not "all" and is unknown, responds with status code Forbidden.
        /// Otherwise:
        ///   Returns all ToDoItems if completedOnly is false and userID is "all"
        ///   Returns all completed ToDoItems if completedOnly is true and userID is "all"
        ///   Returns all of userID's ToDoItems if completedOnly is false 
        ///   Returns userID's completed ToDoItems if completedOnly is true
        ///   In all cases responds with status code OK.
        /// </summary>
        /// <param name="completedOnly">Whether to display tasks by all users</param>
        /// <param name="userID">UserID or "all"</param>
        /// <returns>Requested list of items</returns>
        [Route("ToDo/GetAllItems/{completedOnly}/{userID}")]
        public IList<ToDoItem> GetAllItems(bool completedOnly, string userID)
        {
            lock (sync)
            {
                List<ToDoItem> result = new List<ToDoItem>();
                if (userID != "all" && !users.ContainsKey(userID))
                {
                    throw new HttpResponseException(HttpStatusCode.Forbidden);
                }
                else
                {
                    foreach (ToDoItem item in items.Values)
                    {
                        if (userID != "all" && item.UserID != userID) continue;
                        if (completedOnly && !item.Completed) continue;
                        result.Add(item);
                    }
                    return result;
                }
            }
        }

        // This amounts to a "poor man's" database.  The state of the service is
        // maintained in users and items.  The sync object is used
        // for synchronization (because multiple threads can be running
        // simultaneously in the service).  The entire state is lost each time
        // the service shuts down, so eventually we'll need to port this to
        // a proper database.
        private readonly static Dictionary<String, UserInfo> users = new Dictionary<String, UserInfo>();
        private readonly static Dictionary<String, ToDoItem> items = new Dictionary<String, ToDoItem>();
        private static readonly object sync = new object();
    }
}
