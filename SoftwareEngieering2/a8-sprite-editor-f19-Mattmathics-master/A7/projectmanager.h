#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>

#include "frame.h"
#include "canvas.h"

/**
 * Project Manager Static Class
 *
 * @author Kyungyoon Kim
 */
class ProjectManager
{
public:

    /**
     * Save frame as PNG file.
     *
     * @param frame Frame pointer
     * @param uri Save location
     * @return When success, return true
     */
    static bool saveAsPng(Frame* frame, QString uri);

    /**
     * Save project as strip image (PNG).
     *
     * @param canvas Canvas pointer
     * @param uri Save location
     * @return When success, return true
     */
    static bool saveAsStripImage(Canvas* canvas, QString uri);

    /**
     * Save canvas as GIF file.
     *
     * @param canvas Canvas pointer
     * @param uri Save location
     * @return When success, return true
     */
    static bool saveAsGif(Canvas* canvas, QString uri, int milliseconds);

    /**
     * Save project as SSP file.
     *
     * @param canvas Canvas pointer
     * @param uri Save location
     * @return When success, return true
     */
    static bool saveProject(Canvas* canvas, QString uri);

    /**
     * Open SSP project file.
     *
     * @throws When failed to load project file, throw exception.
     * @param uri Project file URI
     * @return Loaded canvas
     */
    static Canvas openProject(QString uri);

private:
    ProjectManager() {}
};

#endif // PROJECTMANAGER_H
