/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "backupruner.h"

#include "utils/envutil.h"
#include "utils/processutil.h"

#include <QDir>
#include <QDebug>

BackupRuner::BackupRuner(ProductService *product):Runner()
{
    this->m_product = product;
}

void BackupRuner::run()
{
    toSend(m_product->getInfoMsg("message.backupStarting",m_product->title(),m_product->port()));

    QString script = m_product->backkupScript();
    QString folder = m_product->backupPath();

    //check script
    if(script.isEmpty())
    {
        toSend(m_product->getErrorMsg("message.errorBackupScriptNot",m_product->title(),m_product->port()));
        return;
    }

    //check folder
//    if(folder.isEmpty())
//    {
//        sendError("message.errorBackupPathNot",m_product->type(),m_product->title());
//        return;
//    }

    script = EnvUtil::replaceText(script);
    script = EnvUtil::normalizePath(script);

    folder = EnvUtil::replaceText(folder);
    folder = EnvUtil::normalizePath(folder);

    //check folder path
//    QDir toDir(folder);
//    if(toDir.exists() == false)
//    {
//        sendError("message.errorBackupPathNotExist",m_product->type(),m_product->title());
//        return;
//    }

    InfoResult result = ProcessUtil::excuteCommand(script);

    if(result.error == false)
        toSend(m_product->getSuccessMsg("message.backupSuccess",m_product->title(),folder));

    toSendResult(!result.error);
}
