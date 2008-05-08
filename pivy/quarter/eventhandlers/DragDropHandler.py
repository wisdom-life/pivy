###
# Copyright (c) 2002-2008 Kongsberg SIM
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

"""
\class SIM::Coin3D::Quarter::DragDropHandler DragDropHandler.h Quarter/devices/DragDropHandler.h

  \brief The DragDropHandler class provides drag and drop
  functionality to the QuarterWidget. It is not registered with the
  DeviceManager by default.
"""

class DragDropHandler(EventHandler):
    def __init__(self):        
        self._suffixes = ("iv", "wrl")

    """
    Detects a QDragEnterEvent and if the event is the dropping of a
    valid Inventor or VRML it opens the file, reads in the scenegraph
    and calls setSceneGraph on the QuarterWidget
    """
    def handleEvent(event):
        if event.type() == QEvent.DragEnter:
            self._dragEnterEvent(event)
            return true
        elif event.type() == QEvent.Drop:
            self._dropEvent(event)
            return true
        else:
            return false

    def _dragEnterEvent(event):
        mimedata = event.mimeData()
        if not mimedata.hasUrls() and not mimedata.hasText(): return

        if mimedata.hasUrls():
            fileinfo = QFileInfo(mimedata.urls().takeFirst().path())
            suffix = QString(fileinfo.suffix().toLower())

            if not suffix in suffixes: return

        event.acceptProposedAction()

    def _dropEvent(event):
        mimedata = event.mimeData()

        input = SoInput()

        if mimedata.hasUrls():
            url = mimedata.urls().takeFirst()
            if url.scheme().isEmpty() or url.scheme().toLower() == QString("file"):
                # attempt to open file
                if not input.openFile(url.toLocalFile().toLatin1().constData()): return
        elif mimedata.hasText():
            # FIXME 2007-11-09 preng: dropping text buffer does not work on Windows Vista.
            bytes = mimedata.text().toUtf8()
            input.setBuffer(bytes.constData(), bytes.size())
            if not input.isValidBuffer(): return

          # attempt to import it
        root = SoDB.readAll(input)
        if not root: return

        # get QuarterWidget and set new scenegraph
        quarterwidget = self.manager.getWidget()
        quarterwidget.setSceneGraph(root)
        quarterwidget.updateGL()
