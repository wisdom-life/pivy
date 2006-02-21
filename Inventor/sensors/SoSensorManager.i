%{
static void
sensorQueueChangedPythonCB(void * userdata)
{
  PyObject *func, *arglist;
  PyObject *result;

  /* the first item in the userdata sequence is the python callback
   * function; the second is the supplied userdata python object */
  func = PyTuple_GetItem((PyObject *)userdata, 0);
  arglist = Py_BuildValue("(O)", PyTuple_GetItem((PyObject *)userdata, 1));

  if ((result = PyEval_CallObject(func, arglist)) == NULL) {
    PyErr_Print();
  }  
  Py_DECREF(arglist);
  Py_XDECREF(result);
}
%}

/* add python specific callback functions */
%extend SoSensorManager {
  void setChangedCallback(PyObject * pyfunc, PyObject * data) {
    self->setChangedCallback(sensorQueueChangedPythonCB, 
                             (void *)Py_BuildValue("(OO)", 
                                                   pyfunc, 
                                                   data ? data : Py_None));
  }
}
