import pytest
import gc
from ctypes import c_void_p, c_char, c_char_p, c_int, c_longlong, CFUNCTYPE, POINTER, py_object

import yogi

_destroy_mock_fn_keepalive = []


@pytest.fixture
def hello_bytes():
    """Provides a byte array that persists until the test finishes"""
    data = b'hello'
    yield data
    # The data variable should still be alive here


@pytest.fixture
def mocks():
    """Provides Yogi Core API function mocks"""
    mocks_inst = Mocks()
    with mocks_inst:
        yield mocks_inst


@pytest.fixture
def context(mocks):
    """Provides a mocked Context instance"""
    def fn(context):
        context.contents.value = 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ContextCreate(fn)
    return yogi.Context()


@pytest.fixture
def signal_set(mocks, context):
    """Provides a mocked SignalSet instance"""
    def fn(timer, context, signals):
        timer.contents.value = 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_SignalSetCreate(fn)
    return yogi.SignalSet(context, signals=yogi.Signals.TERM)


@pytest.fixture
def configuration(mocks):
    """Provides a mocked Configuration instance"""
    def fn(config, flags):
        config.contents.value = 1234
        return yogi.ErrorCode.OK

    mocks.MOCK_ConfigurationCreate(fn)
    return yogi.Configuration()


class Mocks:
    """All mocks for the API function in Yogi Core"""

    def __enter__(self):
        self._keepalive = []
        self.MOCK_Destroy(lambda _: yogi.ErrorCode.OK)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        yogi._library.yogi_core.MOCK_ResetMocks()
        self._keepalive = []
        self.MOCK_Destroy(lambda _: yogi.ErrorCode.OK)

        # It seems like objects created in a test are being destroyed after the TestCase
        # instance is not alive any more, leading to an invalid handler in MOCK_Destroy()
        _destroy_mock_fn_keepalive.extend(self._keepalive)
        gc.collect()

    # :CODEGEN_BEGIN:

    def MOCK_GetVersion(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_GetVersion
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_CheckBindingsCompatibility(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_CheckBindingsCompatibility
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_GetErrorString(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_GetErrorString
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_char_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_GetLastErrorDetails(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_GetLastErrorDetails
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_GetConstant(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_GetConstant
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_GetSchema(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_GetSchema
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_char_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_GetCurrentTime(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_GetCurrentTime
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_longlong))]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_FormatTime(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_FormatTime
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_longlong, POINTER(c_char_p), POINTER(c_int), c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ParseTime(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ParseTime
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_longlong), c_char_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_FormatDuration(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_FormatDuration
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_longlong, c_int, POINTER(c_char_p), POINTER(c_int), c_char_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_FormatObject(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_FormatObject
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_char_p), POINTER(c_int), c_char_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigureConsoleLogging(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigureConsoleLogging
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_int, c_int, c_int, c_char_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigureHookLogging(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigureHookLogging
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_int, CFUNCTYPE(None, c_int, c_longlong, c_int,
                                                              c_char_p, c_int, c_char_p, c_char_p, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigureFileLogging(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigureFileLogging
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_int, c_char_p, POINTER(c_char_p), POINTER(c_int), c_char_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_LoggerCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_LoggerCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_LoggerGetVerbosity(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_LoggerGetVerbosity
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_int))]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_LoggerSetVerbosity(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_LoggerSetVerbosity
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_LoggerSetComponentsVerbosity(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_LoggerSetComponentsVerbosity
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_char_p, c_int, POINTER(c_int))]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_LoggerLog(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_LoggerLog
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, c_char_p, c_int, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationUpdateFromCommandLine(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationUpdateFromCommandLine
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, POINTER(POINTER(c_char)), c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationUpdateFromJson(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationUpdateFromJson
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationUpdateFromFile(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationUpdateFromFile
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationDump(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationDump
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_char_p), POINTER(c_int), c_int, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationWriteToFile(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationWriteToFile
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_char_p, c_int, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ConfigurationValidate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ConfigurationValidate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_char_p, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p))]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextPoll(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextPoll
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_int))]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextPollOne(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextPollOne
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_int))]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextRun(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextRun
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_int), c_longlong)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextRunOne(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextRunOne
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, POINTER(c_int), c_longlong)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextRunInBackground(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextRunInBackground
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextStop(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextStop
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextWaitForRunning(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextWaitForRunning
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_longlong)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextWaitForStopped(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextWaitForStopped
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_longlong)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_ContextPost(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_ContextPost
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, CFUNCTYPE(None, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_RaiseSignal(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_RaiseSignal
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_int, py_object, CFUNCTYPE(None, py_object, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_SignalSetCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_SignalSetCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_void_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_SignalSetAwaitSignalAsync(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_SignalSetAwaitSignalAsync
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, CFUNCTYPE(None, c_int, c_int, py_object, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_SignalSetCancelAwaitSignal(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_SignalSetCancelAwaitSignal
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_TimerCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_TimerCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_TimerStartAsync(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_TimerStartAsync
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_longlong, CFUNCTYPE(None, c_int, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_TimerCancel(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_TimerCancel
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_void_p, c_void_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchGetInfo(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchGetInfo
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_void_p, POINTER(c_char), c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchGetConnectedBranches(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchGetConnectedBranches
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_void_p, POINTER(
            c_char), c_int, CFUNCTYPE(None, c_int, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchAwaitEventAsync(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchAwaitEventAsync
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, c_void_p, POINTER(
            c_char), c_int, CFUNCTYPE(None, c_int, c_int, c_int, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchCancelAwaitEvent(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchCancelAwaitEvent
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchSendBroadcast(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchSendBroadcast
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, c_void_p, c_int, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchSendBroadcastAsync(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchSendBroadcastAsync
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, c_void_p, c_int, c_int,
                                      CFUNCTYPE(None, c_int, c_int, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchCancelSendBroadcast(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchCancelSendBroadcast
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchReceiveBroadcastAsync(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchReceiveBroadcastAsync
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_void_p, c_int, c_void_p, c_int,
                                      CFUNCTYPE(None, c_int, c_int, c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_BranchCancelReceiveBroadcast(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_BranchCancelReceiveBroadcast
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_Destroy(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_Destroy
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_DestroyAll(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_DestroyAll
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_WebServerCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_WebServerCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_void_p, c_void_p, c_void_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_WebServerAddWorker(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_WebServerAddWorker
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_WebRouteCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_WebRouteCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_void_p, c_char_p, CFUNCTYPE(
            None, c_int, c_int, c_char_p, c_char_p, c_int, c_char_p, POINTER(POINTER(c_char)), c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_WebRouteRespond(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_WebRouteRespond
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, c_int, c_int, c_char_p, c_char_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_WebProcessCreate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_WebProcessCreate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, POINTER(c_void_p), c_void_p, c_char_p, CFUNCTYPE(
            None, c_int, c_int, c_char_p, c_int, c_void_p, POINTER(c_int), c_void_p), c_void_p)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    def MOCK_WebProcessUpdate(self, fn):
        mock_fn = yogi._library.yogi_core.MOCK_WebProcessUpdate
        mock_fn.restype = None
        mock_fn.argtypes = [CFUNCTYPE(c_int, c_void_p, c_int, c_int, c_int, c_void_p, c_int)]
        wrapped_fn = mock_fn.argtypes[0](fn)
        self._keepalive.append(wrapped_fn)
        mock_fn(wrapped_fn)

    # :CODEGEN_END:
