/******************************************************************************
 **                           COPYRIGHT NOTICE
 **      (c) 2012 The Johns Hopkins University Applied Physics Laboratory
 **                         All rights reserved.
 ******************************************************************************/
/*****************************************************************************
 ** \file nm_mgr_db.h
 **
 ** File Name: nm_mgr_db.h
 **
 **
 ** Subsystem:
 **          Network Manager Daemon: Database Utilities
 **
 ** Description: This file implements the DTNMP Manager interface to a back-
 **              end SQL database.
 **
 ** Notes:
 **
 ** Assumptions:
 **
 ** Modification History:
 **  MM/DD/YY  AUTHOR         DESCRIPTION
 **  --------  ------------   ---------------------------------------------
 **  07/10/13  S. Jacobs      Initial Implementation (JHU/APL)
 **  08/19/13  E. Birrane     Documentation clean up and code review comments. (JHU/APL)
 **  08/22/15  E. Birrane     Updates for new schema and dynamic user permissions. (Secure DTN - NASA: NNX14CS58P)
 *****************************************************************************/
#ifdef HAVE_MYSQL

#ifndef NM_MGR_DB_H
#define NM_MGR_DB_H

/* System Headers */
#include "stdio.h"
#include "unistd.h"
#include "mysql.h"

/* ION headers. */
#include "platform.h"
#include "lyst.h"

/* Application headers. */
#include "../shared/adm/adm.h"
#include "../shared/msg/pdu.h"
#include "../shared/primitives/admin.h"
#include "../shared/primitives/mid.h"
#include "../shared/primitives/oid.h"
#include "../shared/primitives/nn.h"
#include "../shared/primitives/report.h"
#include "../shared/primitives/rules.h"
#include "../shared/primitives/ctrl.h"

#include "../shared/utils/db.h"
#include "../shared/utils/ion_if.h"
#include "../shared/utils/utils.h"

#include "nm_mgr_ui.h"
#include "mgr_db.h"

/* Enumerations */

/*
 * Transmit enumerations govern the state associated with messages stored in
 * the database to be sent to an agent. Given that the database may have
 * multiple writers, these states serve as a synchronization mechanism.
 */
#define TX_INIT  (0) /* An outgoing message group is being written to the db. */
#define TX_READY (1) /* An outgoing message group is ready to be processed. */
#define TX_PROC  (2) /* The message group is being processed. */
#define TX_SENT  (3) /* The message group has been processed and sent. */

/*
 * Receive enumerations govern the state associated with messages stored in
 * the database that have been received by an agent.
 */
#define RX_INIT  (0) /* An incoming message group is being received. */
#define RX_READY (1) /* An incoming message group is done being received. */
#define RX_PROC  (2) /* An incoming message group has been processed. */


/*
 * The DB schema uses a table of tables to identify types of information
 * stored in outgoing messages.  These enumerations capture supported
 * table identifiers.
 */

#define UNKNOWN_MSG (0)
#define EXEC_CTRL_MSG (3)

/*
 * Constants relating to how long to try and reconnect to the DB when
 * a connection has failed.
 */

#define SQL_RECONN_TIME_MSEC 500
#define SQL_CONN_TRIES 10


#define CHKCONN db_mgt_connected();


/* Functions to write primitives to associated database tables. */
uint32_t db_add_adm(char *name, char *version, char *oid_root);
uint32_t db_add_agent(eid_t agent_eid);
uint32_t db_add_dc(Lyst dc, ui_parm_spec_t *spec);
uint32_t db_add_mid(mid_t *mid, ui_parm_spec_t *spec, dtnmp_type_e type);
uint32_t db_add_mc(Lyst mc);
uint32_t db_add_nn(oid_nn_t *nn);
uint32_t db_add_oid(oid_t *oid);
uint32_t db_add_parms(oid_t *oid, ui_parm_spec_t *spec);
uint32_t db_add_protomid(mid_t *mid, ui_parm_spec_t *spec, dtnmp_type_e type);
uint32_t db_add_protoparms(ui_parm_spec_t *spec);



/* Functions to fetch primitives from associated database tables. */

uint32_t          db_fetch_adm_idx(char *name, char *version);
Lyst              db_fetch_dc(int dc_idx);
blob_t*           db_fetch_data_col_entry_from_row(MYSQL_ROW row);
mid_t*            db_fetch_mid(int idx);
Lyst              db_fetch_mid_col(int idx);
uint32_t          db_fetch_mid_idx(mid_t *mid);
uint32_t          db_fetch_nn(uint32_t idx);
uint32_t          db_fetch_nn_idx(uint32_t nn);
uint8_t*          db_fetch_oid_val(uint32_t idx, uint32_t *size);
oid_t*            db_fetch_oid(uint32_t nn_idx, uint32_t parm_idx, uint32_t oid_idx);
uint32_t          db_fetch_oid_idx(oid_t *oid);
Lyst			  db_fetch_parms(uint32_t idx);
uint32_t          db_fetch_protomid_idx(mid_t *mid);
adm_reg_agent_t  *db_fetch_reg_agent(uint32_t id);
uint32_t          db_fetch_reg_agent_idx(eid_t *sender);


/* Functions to process incoming messages. */
int db_incoming_initialize(time_t timestamp, eid_t *sender_eid);
int db_incoming_finalize(uint32_t incomingID);
int db_incoming_process_message(int incomingID, uint8_t *cursor, uint32_t size);

/* Database Management Functions. */
void    *db_mgt_daemon(int *running);
uint32_t db_mgt_init(ui_db_t parms, uint32_t clear);
int      db_mgt_clear();
int      db_mgt_clear_table(char *table);
void     db_mgt_close();
int      db_mgt_connected();
void     db_mgt_verify_mids();

/* Functions to process outgoing message tables. */
int  db_outgoing_process(MYSQL_RES *sql_res);
int  db_outgoing_process_messages(uint32_t idx, pdu_group_t *msg_group);
int  db_outgoing_process_one_message(uint32_t idx, uint32_t entry_idx, pdu_group_t *msg_group, MYSQL_ROW row, Lyst defs);
Lyst db_outgoing_process_recipients(uint32_t id);
int  db_outgoing_ready(MYSQL_RES **sql_res);


#endif

#endif // HAVE_MYSQL


