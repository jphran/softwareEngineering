using System;
using System.Windows.Forms;

namespace ToDoListClient
{
    /// <summary>
    /// Interface implemented by <see cref="ToDoListView"/>
    /// </summary>
    public interface IToDoListView
    {
        /// <summary>
        /// If state == true, enables all controls that are normally enabled; disables Cancel.
        /// If state == false, disables all controls; enables Cancel.
        /// </summary>
        void EnableControls(bool state);

        /// <summary>
        /// Removes all displayed tasks.
        /// </summary>
        void Clear();

        /// <summary>
        /// Adds a row to the task display.
        /// </summary>
        void AddItem(string description, bool isCompleted, bool belongsToUser);

        /// <summary>
        /// Is the user currently registered?
        /// </summary>
        bool IsUserRegistered { get; set; }

        /// <summary>
        /// Fired when user must be registered.
        /// Parameters are name and email.
        /// </summary>
        event Action<string, string> RegisterPressed;

        /// <summary>
        /// Fired when new task must be submitted.
        /// Parameter is the task description.
        /// </summary>
        event Action<string> SubmitPressed;

        /// <summary>
        /// Fired when one of the filter checkboxes is changed.
        /// Parameters are the states of the checkboxes.
        /// </summary>
        event Action<bool, bool> FilterChanged;

        /// <summary>
        /// Fires when a task is to be deleted.
        /// Parameter is the line on which the task appears.
        /// </summary>
        event Action<int> DeletePressed;

        /// <summary>
        /// Fires when a task is to marked as completed.
        /// Parameter is the line on which the task appears.
        /// </summary>
        event Action<int> DonePressed;

        /// <summary>
        /// Fires when an ongoing action must be canceled.
        /// </summary>
        event Action CancelPressed;
    }
}
