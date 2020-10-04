/*
 * This file is part of the Yogi Framework
 * https://github.com/yohummus/yogi-framework.
 *
 * Copyright (c) 2020 Johannes Bergmann.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

using System;

public static partial class Yogi
{
    /// <summary>
    /// Error codes returned by functions from the Yogi Core library.
    /// </summary>
    public enum ErrorCode
    {
        // :CODEGEN_BEGIN: gen_dotnet_enums

        /// <summary>Success</summary>
        Ok = 0,

        /// <summary>Unknown internal error occured</summary>
        Unknown = -1,

        /// <summary>The object is still being used by another object</summary>
        ObjectStillUsed = -2,

        /// <summary>Insufficient memory to complete the operation</summary>
        BadAlloc = -3,

        /// <summary>Invalid parameter</summary>
        InvalidParam = -4,

        /// <summary>Invalid Handle</summary>
        InvalidHandle = -5,

        /// <summary>Object is of the wrong type</summary>
        WrongObjectType = -6,

        /// <summary>The operation has been canceled</summary>
        Canceled = -7,

        /// <summary>Operation failed because the object is busy</summary>
        Busy = -8,

        /// <summary>The operation timed out</summary>
        Timeout = -9,

        /// <summary>The timer has not been started or already expired</summary>
        TimerExpired = -10,

        /// <summary>The supplied buffer is too small</summary>
        BufferTooSmall = -11,

        /// <summary>Could not open a socket</summary>
        OpenSocketFailed = -12,

        /// <summary>Could not bind a socket</summary>
        BindSocketFailed = -13,

        /// <summary>Could not listen on socket</summary>
        ListenSocketFailed = -14,

        /// <summary>Could not set a socket option</summary>
        SetSocketOptionFailed = -15,

        /// <summary>Invalid regular expression</summary>
        InvalidRegex = -16,

        /// <summary>Could not read from file</summary>
        ReadFileFailed = -17,

        /// <summary>Could not read from or write to socket</summary>
        RwSocketFailed = -18,

        /// <summary>Could not connect a socket</summary>
        ConnectSocketFailed = -19,

        /// <summary>The magic prefix sent when establishing a connection is wrong</summary>
        InvalidMagicPrefix = -20,

        /// <summary>The Yogi versions are not compatible</summary>
        IncompatibleVersion = -21,

        /// <summary>Could not deserialize a message</summary>
        DeserializeMsgFailed = -22,

        /// <summary>Could not accept a socket</summary>
        AcceptSocketFailed = -23,

        /// <summary>Attempting to connect branch to itself</summary>
        LoopbackConnection = -24,

        /// <summary>The passwords of the local and remote branch do not match</summary>
        PasswordMismatch = -25,

        /// <summary>The net names of the local and remote branch do not match</summary>
        NetNameMismatch = -26,

        /// <summary>A branch with the same name is already active</summary>
        DuplicateBranchName = -27,

        /// <summary>A branch with the same path is already active</summary>
        DuplicateBranchPath = -28,

        /// <summary>Message payload is too large</summary>
        PayloadTooLarge = -29,

        /// <summary>Parsing the command line failed</summary>
        ParsingCmdlineFailed = -30,

        /// <summary>Parsing a JSON string failed</summary>
        ParsingJsonFailed = -31,

        /// <summary>Parsing a configuration file failed</summary>
        ParsingFileFailed = -32,

        /// <summary>The configuration is not valid</summary>
        ConfigNotValid = -33,

        /// <summary>Help/usage text requested</summary>
        HelpRequested = -34,

        /// <summary>Could not write to file</summary>
        WriteFileFailed = -35,

        /// <summary>One or more configuration variables are undefined or could not be resolved</summary>
        UndefinedVariables = -36,

        /// <summary>Support for configuration variables has been disabled</summary>
        NoVariableSupport = -37,

        /// <summary>A configuration variable has been used in a key</summary>
        VariableUsedInKey = -38,

        /// <summary>Invalid time format</summary>
        InvalidTimeFormat = -39,

        /// <summary>Could not parse time string</summary>
        ParsingTimeFailed = -40,

        /// <summary>A send queue for a remote branch is full</summary>
        TxQueueFull = -41,

        /// <summary>Invalid operation ID</summary>
        InvalidOperationId = -42,

        /// <summary>Operation is not running</summary>
        OperationNotRunning = -43,

        /// <summary>User-supplied data is not valid MessagePack</summary>
        InvalidUserMsgpack = -44,

        /// <summary>Joining UDP multicast group failed</summary>
        JoinMulticastGroupFailed = -45,

        /// <summary>Enumerating network interfaces failed</summary>
        EnumerateNetworkInterfacesFailed = -46,

        /// <summary>The section could not be found in the configuration</summary>
        ConfigurationSectionNotFound = -47,

        /// <summary>Validating the configuration failed</summary>
        ConfigurationValidationFailed = -48,

        /// <summary>The context has already been added as a worker</summary>
        WorkerAlreadyAdded = -49,

        /// <summary>Could not open file</summary>
        OpenFileFailed = -50,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Levels of how verbose logging output is.
    /// </summary>
    public enum Verbosity
    {
        // :CODEGEN_BEGIN:

        /// <summary>Used to disable logging</summary>
        None = -1,

        /// <summary>Errors that require a process restart</summary>
        Fatal = 0,

        /// <summary>Errors that the system can recover from</summary>
        Error = 1,

        /// <summary>Warnings</summary>
        Warning = 2,

        /// <summary>General information about the system state</summary>
        Info = 3,

        /// <summary>Information for debugging</summary>
        Debug = 4,

        /// <summary>Detailed debugging</summary>
        Trace = 5,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Output streams for writing to the terminal.
    /// </summary>
    public enum Stream
    {
        // :CODEGEN_BEGIN:

        /// <summary>Standard output</summary>
        Stdout = 0,

        /// <summary>Standard error output</summary>
        Stderr = 1,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Built-in JSON Schemas.
    ///
    /// JSON Schemas used for describing and validating various forms of data such
    /// as configurations or results passed back to the library user (e.g. branch
    /// information).
    /// </summary>
    public enum Schema
    {
        // :CODEGEN_BEGIN:

        /// <summary>All possible branch properties</summary>
        BranchProperties = 0,

        /// <summary>Properties for creating a branch</summary>
        BranchConfig = 1,

        /// <summary>Information about a local branch</summary>
        LocalBranchInfo = 2,

        /// <summary>Information about a remote branch</summary>
        RemoteBranchInfo = 3,

        /// <summary>Information about a branch event</summary>
        BranchEvent = 4,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Possible data/payload encoding types.
    /// </summary>
    public enum Encoding
    {
        // :CODEGEN_BEGIN:

        /// <summary>Data is encoded as JSON</summary>
        Json = 0,

        /// <summary>Data is encoded as MessagePack</summary>
        Msgpack = 1,

        // :CODEGEN_END:
    }

    /// <summary>
    /// HTTP response status codes.
    /// </summary>
    public enum HttpStatus
    {
        // :CODEGEN_BEGIN:

        /// <summary>OK</summary>
        Http200 = 200,

        /// <summary>Created</summary>
        Http201 = 201,

        /// <summary>Accepted</summary>
        Http202 = 202,

        /// <summary>Non-Authoritative Information</summary>
        Http203 = 203,

        /// <summary>No Content</summary>
        Http204 = 204,

        /// <summary>Reset Content</summary>
        Http205 = 205,

        /// <summary>Partial Content</summary>
        Http206 = 206,

        /// <summary>Multiple Choices</summary>
        Http300 = 300,

        /// <summary>Moved Permanently</summary>
        Http301 = 301,

        /// <summary>Found</summary>
        Http302 = 302,

        /// <summary>See Other</summary>
        Http303 = 303,

        /// <summary>Not Modified</summary>
        Http304 = 304,

        /// <summary>Temporary Redirect</summary>
        Http307 = 307,

        /// <summary>Permanent Redirect</summary>
        Http308 = 308,

        /// <summary>Bad Request</summary>
        Http400 = 400,

        /// <summary>Unauthorized</summary>
        Http401 = 401,

        /// <summary>Forbidden</summary>
        Http403 = 403,

        /// <summary>Not Found</summary>
        Http404 = 404,

        /// <summary>Method Not Allowed</summary>
        Http405 = 405,

        /// <summary>Not Acceptable</summary>
        Http406 = 406,

        /// <summary>Proxy Authentication Required</summary>
        Http407 = 407,

        /// <summary>Request Timeout</summary>
        Http408 = 408,

        /// <summary>Conflict</summary>
        Http409 = 409,

        /// <summary>Gone</summary>
        Http410 = 410,

        /// <summary>Length Required</summary>
        Http411 = 411,

        /// <summary>Precondition Failed</summary>
        Http412 = 412,

        /// <summary>Payload Too Large</summary>
        Http413 = 413,

        /// <summary>URI Too Long</summary>
        Http414 = 414,

        /// <summary>Unsupported Media Type</summary>
        Http415 = 415,

        /// <summary>Range Not Satisfiable</summary>
        Http416 = 416,

        /// <summary>Expectation Failed</summary>
        Http417 = 417,

        /// <summary>I am a teapot</summary>
        Http418 = 418,

        /// <summary>Unprocessable Entity</summary>
        Http422 = 422,

        /// <summary>Too Early</summary>
        Http425 = 425,

        /// <summary>Upgrade Required</summary>
        Http426 = 426,

        /// <summary>Precondition Required</summary>
        Http428 = 428,

        /// <summary>Too Many Requests</summary>
        Http429 = 429,

        /// <summary>Request Header Fields Too Large</summary>
        Http431 = 431,

        /// <summary>Unavailable For Legal Reasons</summary>
        Http451 = 451,

        /// <summary>Internal Server Error</summary>
        Http500 = 500,

        /// <summary>Not Implemented</summary>
        Http501 = 501,

        /// <summary>Bad Gateway</summary>
        Http502 = 502,

        /// <summary>Service Unavailable</summary>
        Http503 = 503,

        /// <summary>Gateway Timeout</summary>
        Http504 = 504,

        /// <summary>HTTP Version Not Supported</summary>
        Http505 = 505,

        /// <summary>Network Authentication Required</summary>
        Http511 = 511,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Actions on a web process, originating from a web client.
    /// </summary>
    public enum WebProcessAction
    {
        // :CODEGEN_BEGIN:

        /// <summary>Client requests to start a process</summary>
        Start = 0,

        /// <summary>Client sent some input to the process</summary>
        Input = 1,

        /// <summary>Client canceled the process</summary>
        Canceled = 2,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Updates from a web process, originating from the web server.
    /// </summary>
    public enum WebProcessUpdate
    {
        // :CODEGEN_BEGIN:

        /// <summary>Process has been started</summary>
        Started = 0,

        /// <summary>Process could not be started</summary>
        FailedToStart = 1,

        /// <summary>Process finished gracefully</summary>
        Finished = 2,

        /// <summary>Process finished by crashing</summary>
        Crashed = 3,

        /// <summary>Process finished by cancelation</summary>
        Canceled = 4,

        /// <summary>Process created some output</summary>
        Output = 5,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Signals.
    /// </summary>
    [Flags]
    public enum Signals
    {
        // :CODEGEN_BEGIN:

        /// <summary>No signal</summary>
        None = 0,

        /// <summary>Interrupt (e.g. by pressing STRG + C)</summary>
        Int = 1 << 0,

        /// <summary>Termination request</summary>
        Term = 1 << 1,

        /// <summary>User-defined signal 1</summary>
        Usr1 = 1 << 24,

        /// <summary>User-defined signal 2</summary>
        Usr2 = 1 << 25,

        /// <summary>User-defined signal 3</summary>
        Usr3 = 1 << 26,

        /// <summary>User-defined signal 4</summary>
        Usr4 = 1 << 27,

        /// <summary>User-defined signal 5</summary>
        Usr5 = 1 << 28,

        /// <summary>User-defined signal 6</summary>
        Usr6 = 1 << 29,

        /// <summary>User-defined signal 7</summary>
        Usr7 = 1 << 30,

        /// <summary>User-defined signal 8</summary>
        Usr8 = 1 << 31,

        /// <summary>All signals</summary>
        All = Int | Term | Usr1 | Usr2 | Usr3 | Usr4 | Usr5 | Usr6 | Usr7 | Usr8,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Flags used to change a configuration object's behaviour.
    /// </summary>
    [Flags]
    public enum ConfigurationFlags
    {
        // :CODEGEN_BEGIN:

        /// <summary>No flags</summary>
        None = 0,

        /// <summary>
        /// Disables support for variables in the configuration
        /// 
        /// Variables are used to define common values (or parts of values) in a
        /// pre-defined  section. They can then be used in multiple other parts of the
        /// configuration.
        /// 
        /// \note
        ///   Variables can *not* be used in keys.
        /// 
        /// Variables are defined in the *variables* section of the configuration:
        /// 
        /// \code
        /// {
        ///   "variables": {
        ///     "ROOT":         "/usr/share/my-app",
        ///     "DATA_DIR":     "${ROOT}/data"
        ///     "DURATION":     30,
        ///     "MAX_DURATION": "${DURATION}",
        /// ...
        /// \endcode
        /// These variables can then be used anywhere in the configuration, even in the
        /// *variables* section itself as shown above.
        /// 
        /// \note
        ///   Even if the value of a variable is not a string, the placeholder *${name}*
        ///   always has to be a string to conform to the JSON format. When resolving a
        ///   placeholder for a non-string variable, the type of the target value will
        ///   be changed accordingly if and only if the placeholder is surrounded by
        ///   quotation marks as shown for the *MAX_DURATION* variable above. Otherwise,
        ///   the target value will remain a string and the placeholder will be replaced
        ///   with the stringified value of the variable.
        /// </summary>
        DisableVariables = 1 << 0,

        /// <summary>
        /// Makes configuration options given directly on the command line overridable
        /// 
        /// By default, configuration options given direclty on the command line are
        /// immutable, i.e. they will never be updated from another source.
        /// </summary>
        MutableCmdLine = 1 << 1,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Flags used to adjust how command line options are parsed.
    /// </summary>
    [Flags]
    public enum CommandLineOptions
    {
        // :CODEGEN_BEGIN:

        /// <summary>No options</summary>
        None = 0,

        /// <summary>Include the --log... switches for configuring file logging</summary>
        Logging = 1 << 0,

        /// <summary>Include the --name switch for setting the branch name</summary>
        BranchName = 1 << 1,

        /// <summary>Include the --description switch for setting the branch description</summary>
        BranchDescription = 1 << 2,

        /// <summary>Include the --network switch for setting the network name</summary>
        BranchNetwork = 1 << 3,

        /// <summary>Include the --password switch for setting the network password</summary>
        BranchPassword = 1 << 4,

        /// <summary>Include the --path switch for setting the branch path</summary>
        BranchPath = 1 << 5,

        /// <summary>Include the --adv-interfaces switch for setting the branch advertising interfaces</summary>
        BranchAdvIfs = 1 << 6,

        /// <summary>Include the --adv-addr switch for setting the branch advertising address</summary>
        BranchAdvAddr = 1 << 7,

        /// <summary>Include the --adv-port switch for setting the branch advertising port</summary>
        BranchAdvPort = 1 << 8,

        /// <summary>Include the --adv-int switch for setting the branch advertising interval</summary>
        BranchAdvInt = 1 << 9,

        /// <summary>Include the --timeout switch for setting the branch timeout</summary>
        BranchTimeout = 1 << 10,

        /// <summary>Include the --ghost_mode switch for enabling ghost mode for the branch</summary>
        BranchGhostMode = 1 << 11,

        /// <summary>
        /// Parse configuration files given on the command line
        /// 
        /// The files will be parsed from left to right, i.e. if the same value is set
        /// in two supplied configuration files, then the value from the rightmost file
        /// will be used. However, values given directly on the command line, i.e. not
        /// through files, have higher priority.
        /// </summary>
        Files = 1 << 12,

        /// <summary>At least one configuration file must be given</summary>
        FilesRequired = 1 << 13,

        /// <summary>
        /// Include the --override switch for overriding arbitrary configuration sections
        /// 
        /// This is useful for supplying arbitrary parameters on the command line
        /// without having to store them in a file.
        /// 
        /// \note
        ///   Parameters supplied in this way override the same parameters in any
        ///   given configuration file. If the same parameter is set directly on the
        ///   command line multiple times, then the rightmost value is used.
        /// </summary>
        Overrides = 1 << 14,

        /// <summary>Include the --var switch for setting variables</summary>
        Variables = 1 << 15,

        /// <summary>Combination of all branch flags</summary>
        BranchAll = BranchName | BranchDescription | BranchNetwork | BranchPassword | BranchPath | BranchAdvIfs | BranchAdvAddr | BranchAdvPort | BranchAdvInt | BranchTimeout | BranchGhostMode,

        /// <summary>
        /// Combination of all flags
        /// 
        /// This is usually used with the application object.
        /// </summary>
        All = Logging | BranchAll | Files | FilesRequired | Overrides | Variables,

        // :CODEGEN_END:
    }

    /// <summary>
    /// Events that can be observed on a branch.
    /// </summary>
    [Flags]
    public enum BranchEvents
    {
        // :CODEGEN_BEGIN:

        /// <summary>No event (passed to event handler function if wait operation failed)</summary>
        None = 0,

        /// <summary>
        /// A new branch has been discovered
        /// 
        /// A remote branch is considered to be a _new branch_ if it is neither already
        /// connected nor in the process of being connected to. This means if we
        /// discover a branch and connect to it but connecting to it fails and the
        /// remote branch itself has not connected to us, then the next time an
        /// we receive an advertisement message we consider the branch new again.
        /// 
        /// Associated event information:
        /// 
        /// \code
        ///   {
        ///     "uuid":               "123e4567-e89b-12d3-a456-426655440000",
        ///     "tcp_server_address": "fe80::f086:b106:2c1b:c45",
        ///     "tcp_server_port":    43384
        ///   }
        /// \endcode
        /// </summary>
        BranchDiscovered = 1 << 0,

        /// <summary>
        /// Querying a new branch for information finished (successfully or not)
        /// 
        /// Associated event information:
        /// 
        /// \code
        ///   {
        ///     "uuid":                 "123e4567-e89b-12d3-a456-426655440000",
        ///     "name":                 "Pump Safety Logic",
        ///     "description":          "Monitors the pump for safety",
        ///     "network_name":         "Hardware Control",
        ///     "path":                 "/Cooling System/Pump/Safety",
        ///     "hostname":             "beaglebone",
        ///     "pid":                  3321,
        ///     "tcp_server_address":   "fe80::f086:b106:2c1b:c45",
        ///     "tcp_server_port":      43384,
        ///     "start_time":           "2018-04-23T18:25:43.511Z",
        ///     "timeout":              3.0,
        ///     "advertising_interval": 1.0,
        ///     "ghost_mode":           false
        ///   }
        /// \endcode
        /// </summary>
        BranchQueried = 1 << 1,

        /// <summary>
        /// Connecting to a branch finished (successfully or not)
        /// 
        /// Associated event information:
        /// 
        /// \code
        ///   {
        ///     "uuid": "123e4567-e89b-12d3-a456-426655440000"
        ///   }
        /// \endcode
        /// </summary>
        ConnectFinished = 1 << 2,

        /// <summary>
        /// The connection to a branch was lost
        /// 
        /// Associated event information:
        /// 
        /// \code
        ///   {
        ///     "uuid": "123e4567-e89b-12d3-a456-426655440000"
        ///   }
        /// \endcode
        /// </summary>
        ConnectionLost = 1 << 3,

        /// <summary>All branch events</summary>
        All = BranchDiscovered | BranchQueried | ConnectFinished | ConnectionLost,

        // :CODEGEN_END:
    }

    /// <summary>
    /// HTTP request methods.
    /// </summary>
    [Flags]
    public enum HttpMethods
    {
        // :CODEGEN_BEGIN:

        /// <summary>No methods</summary>
        None = 0,

        /// <summary>GET method for retrieving data</summary>
        Get = 1 << 0,

        /// <summary>
        /// HEAD method for retrieving the same header as with the GET request but
        /// without the response body
        /// </summary>
        Head = 1 << 1,

        /// <summary>POST method for submitting data</summary>
        Post = 1 << 2,

        /// <summary>PUT method for modifying data</summary>
        Put = 1 << 3,

        /// <summary>DELETE method for deleting data</summary>
        Delete = 1 << 4,

        /// <summary>PATCH method for partially modifying data</summary>
        Patch = 1 << 5,

        // :CODEGEN_END:
    }
}
