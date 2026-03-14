using System;

namespace Pixelpart
{
    /// <summary>
    /// Event args for effect events
    /// </summary>
    public class EffectEventArgs : EventArgs
    {
        /// <summary>
        /// Id of invoked effect event
        /// </summary>
        public uint EventId { get; set; }

        /// <summary>
        /// Name of invoked effect event
        /// </summary>
        public string EventName { get; set; }
    }
}
