// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_ISSUES_ISSUES_BACKEND_HPP_
#define CLUSTERING_ADMINISTRATION_ISSUES_ISSUES_BACKEND_HPP_

#include <string>
#include <vector>

#include "errors.hpp"
#include <boost/shared_ptr.hpp>

#include "rdb_protocol/artificial_table/backend.hpp"
#include "clustering/administration/metadata.hpp"
#include "concurrency/watchable.hpp"
#include "clustering/administration/issues/remote.hpp"
#include "clustering/administration/issues/name_collision.hpp"


class issues_artificial_table_backend_t :
    public artificial_table_backend_t
{
public:
    issues_artificial_table_backend_t(
        boost::shared_ptr<semilattice_read_view_t<cluster_semilattice_metadata_t> >
            _cluster_sl_view,
        const clone_ptr_t<watchable_t<change_tracking_map_t<peer_id_t,
            cluster_directory_metadata_t> > >
                &directory_view);

    std::string get_primary_key_name();

    bool read_all_rows_as_vector(signal_t *interruptor,
                                 std::vector<ql::datum_t> *rows_out,
                                 std::string *error_out);

    bool read_row(ql::datum_t primary_key,
                  signal_t *interruptor,
                  ql::datum_t *row_out,
                  std::string *error_out);

    bool write_row(ql::datum_t primary_key,
                   bool pkey_was_autogenerated,
                   ql::datum_t new_value,
                   signal_t *interruptor,
                   std::string *error_out);

private:
    std::vector<scoped_ptr_t<issue_t> > all_issues() const;

    boost::shared_ptr<semilattice_read_view_t<cluster_semilattice_metadata_t> >
        cluster_sl_view;

    std::set<issue_tracker_t *> trackers;

    // Global issues are tracked here, local issues are collected by
    // the remote_issue_tracker out of the directory metadata
    remote_issue_tracker_t remote_issue_tracker;
    name_collision_issue_tracker_t name_collision_issue_tracker;
};

#endif /* CLUSTERING_ADMINISTRATION_ISSUES_ISSUES_BACKEND_HPP_ */
